// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"
#include "CSVWorker.h"
#include "INIReader.h"
#include "MarkGenerator.h"

#include "MProcUtils.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	int mode=1;
	int radar = 1;
	vector<bool> settings;
	//TODO ADD pre-load CSV
	CSVWorker csvw;
	day = 1;
	stringstream ss;

	if(argc <= 1)
    {
        std::cout << "usage: " << argv[0] << " filename" << std::endl;
		std::cout << "usage: " << argv[0] << " yyyy iiiii mm" << std::endl;
        return 0;
    }

	INIReader reader("setup.ini");

	if (reader.ParseError() < 0) 
	{
		std::cout << "Can't load 'setup.ini'\n";
		for (i = 0; i != NUMPARAMETERS_STR + NUMPARAMETERS; i++)
		{
			settings.push_back(true);
		}
	}
	else
	{
		for (i = 0; i != NUMPARAMETERS_STR + NUMPARAMETERS; i++)
		{
			ss << "param" << i;
			bool data = reader.GetBoolean("settings", ss.str(), true);
			settings.push_back(data);
			ss.clear();
			ss = stringstream();
		}

		MarkGen& mg = MarkGen::Instance();
		for (i = 0; i != MARKS_NUMBER; i++)
		{
			ss << "param" << i;
			mg.setMarkEnable(i, reader.GetBoolean("MarkGenerator", ss.str(), true));
			ss.clear();
			ss = stringstream();
		}

		for (i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			ss << "param" << i;
			mg.setDispersionCalcThreshold(i, reader.GetReal("DispersionCalculator", ss.str(), 1.0f));
			ss.clear();
			ss = stringstream();
		}
	}

	

	string infile,infile2;
	string outfile;
	
	std::istringstream oss;
	size_t pos;



	switch (argc)
	{
	case 2:
		infile = string(argv[1]);
		pos = infile.find_last_of('/');
		if (pos == std::string::npos)
			pos = 0;
		oss = std::istringstream(infile.substr(pos, infile.find('-')));
		oss >> station_index;
		oss.clear();
		oss = std::istringstream(infile.substr(infile.find('-') + 1, 4));
		oss >> year;
		oss.clear();
		oss = std::istringstream(infile.substr(infile.find('-') + 5, 2));
		oss >> month;
		break;
	case 4:
	case 6:

		for (i = 1; i != argc; i++)
			recognizeToken(string(argv[i]));

		ss << s_year << '/' << s_station_index << '/' << s_station_index << '-' << s_year << s_month;// << radar1[radar] << ".zip";
		infile = ss.str();
		ss.clear();
		ss = stringstream();
		ss << s_year << '/' << s_station_index << '/' << s_station_index << ' ' << s_year << s_month;// << radar1[radar] << ".zip";
		infile2 = ss.str();

		oss = std::istringstream(argv[1]);
		oss >> year;
		oss.clear();
		oss = std::istringstream(argv[3]);
		oss >> month;
		oss.clear();
		oss = std::istringstream(argv[2]);
		oss >> station_index;
		oss.clear();
		
		break;
	default:
		break;
	}
	
	string datam[N];
	string datan[N];
	string data;

	ss.clear();
	ss = stringstream();
	ss << s_station_index << '-' << s_year << s_month<<".csv";
	outfile = outdir+ss.str();
	infile = curdir + infile;

	string morningstr;
	string nightstr;
	try
	{
		//TODO CHECK DATA ALREADY EXISTS!!! - OK
		try
		{
			csvw.readCSV(outfile);
		}
		catch (...)
		{
			ss << "ERROR Reading input file" << endl;
		}

		
		for (radar = 0; radar != NUMRADARS; radar++)
		{
			try
			{
				string test_pr = infile;
				if (!fileExists(infile + radar2[radar]))
					test_pr = infile2;
				zip_file file(test_pr + radar2[radar]);
				string base = file.get_first_filename();
				if (base.find_last_of('/') != string::npos)
					base = base.substr(0, base.find_last_of('/')) + "/";
				else
					base = "";
				ss << base << day << '.' << month << '.' << year;
				//base = ss.str();

				for (day = 1; day <= daysInMonth(month); day++)
				{
					Sounding s;
					s.setRAWDataIdentifier(raw_radar[radar]);
					ss.clear();
					ss = stringstream();

					if (radar != 2)
					{
						ss << base << day << '.' << month << '.' << year;// << "-11.30";
					}
					else
					{
						char date[10];
						sprintf_s(date, "%04d%02d%02d", year, month, day);
						ss << base << string(date);
					}

					morningstr = ss.str();
					nightstr = morningstr;
					
					cout << morningstr << endl;
					for (i = 0; i != N; i++)
					{
						s.setDayOrNight(0);
						try
						{
							data = readFormat(file, morningstr, 0, formats[i]);//file.read(morningstr + formats[i]);
							datam[i] = data;
							s.addData(data, formats[i]);
						}
						catch (...)
						{
							cout << "#2 Error reading format " << formats[i] << endl;
							datam[i] = "";
							s.addData("", formats[i]);
						}
						s.setDayOrNight(1);
						try
						{
							data = readFormat(file, nightstr, 1, formats[i]);
							datan[i] = data;
							s.addData(data, formats[i]);
						}
						catch (...)
						{
							cout << "#3 Error reading format " << formats[i] << endl;
							datan[i] = "";
							s.addData("", formats[i]);
						}
					}

					LaunchTime lt1;
					lt1.tm_hour = 11;
					lt1.tm_min = 30;
					LaunchTime lt2;
					lt2.tm_hour = 23;
					lt2.tm_min = 30;

					for (int cnt = 0; cnt <= 1; cnt++)
					{
						s.setDayOrNight(cnt);
						s.processRAWFile();
						s.processINFOFile();
						if (settings[NUMPARAMETERS_STR + 10])
							s.processTAE3File();
						if (settings[NUMPARAMETERS_STR + 2])
							s.processKN04File();
						//yyyy.mm.dd hh:mm
						LaunchTime lt;
						lt1.tm_year = year;
						lt1.tm_mon = month;
						lt1.tm_day = day;
						lt2.tm_year = year;
						lt2.tm_mon = month;
						lt2.tm_day = day;
						if (cnt == 0)
						{
							lt = lt1;
						}
						else
						{
							lt = lt2;
						}
						
						MarkGen& mg = MarkGen::Instance();

						LaunchParameters l;
						l.radarCode = radar1[radar];
						//l.filesAvail = s.getFormatsTelegram();

						int t1 = s.getRAWSoundingTime();
						int t2 = s.getSoundingTime();
						
						mg.setSoundingTime(t2);

						for (int st = 0; st != NUMPARAMETERS_STR; st++)
						{
							if (settings[st])
							{
								switch (st)
								{
									case 0:
										l.strparams.push_back(s.getFormatsTelegram() + error_str);
										break;
									default:
										break;
								}
							}
							else
							{
								l.strparams.push_back("//");
							}
						}
						double alt;
						for (int st = 0; st != NUMPARAMETERS; st++)
						{
							if (settings[st + NUMPARAMETERS_STR])
							{
								switch (st)
								{
								case 0:
									l.params.push_back((double)s.getRAWSoundingTime());
									break;
								case 1:
									alt = (double)s.getMaxAltitude();
									mg.setMaxAltitude((int)alt);
									l.params.push_back(alt);
									break;
								case 2:
									l.params.push_back((double)s.getKN04Code());
									break;
								case 3:
									l.params.push_back((double)s.getMaxDistance());
									break;
								case 4:
									l.params.push_back((double)s.getMinElevation());
									break;
								case 5:
									l.params.push_back((double)s.getAlt10Elevation());
									break;
								case 6:
									l.params.push_back((double)mg.getQualityMark());
									break;
								case 7:
									l.params.push_back((double)s.getGroundTemperature());
									break;
								case 8:
									l.params.push_back((double)s.getMinTemperature());
									break;
								case 9:
									l.params.push_back((double)s.getInfoZondType());
									break;
								case 10:
									l.params.push_back((double)s.getAverageWindDirection());
									break;
								case 11:
									l.params.push_back((double)s.getAverageWindSpeed());
									break;
								default:
									break;
								}
							}
							else
							{
								l.params.push_back(0.0f);
							}
						}

						if (t1 || t2)
							csvw.addLaunch(cnt, lt, l);

						cout << "-----------------------------------\n SUMMARY INFORMATION"<< cnt << endl;
						cout << "Formats " << s.checkFormats() << " of 14" << endl;
						cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
						cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
						cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
						cout << "Max distance(RAW) " << s.getMaxDistance() << endl;
						mg.clearCalcs();
					}
				}

			}
			catch (...)
			{
				cout << "#1 Error reading file " << infile + radar2[radar] << endl;
			}
		}
		csvw.writeCSV(outfile);
	}
	catch (...)
	{
		cout << "#4 Unknown error " << infile + radar2[radar] << endl;
	}
	system("pause");
	return 0;
}

