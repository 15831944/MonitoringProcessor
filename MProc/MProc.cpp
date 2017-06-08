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
	//TODO ADD pre-load CSV
	
	day = 1;
	stringstream ss;
	std::istringstream oss;
	size_t pos;
	string datam[N];
	string datan[N];
	string data;

	if (!printUsageStrings(argc, argv))
		return 0;

	readSetupFile();
	
	processInputParameters(argc, argv);
	
	makeInOutFilenames();

	string morningstr;
	string nightstr;
	try
	{
		//TODO CHECK DATA ALREADY EXISTS!!! - OK
		readCSVFile();

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
						
						int t1 = s.getRAWSoundingTime();
						int t2 = s.getSoundingTime();
						
						mg.setSoundingTime(t2);

						push_back_params(l, s, settings, error_str);

						if (t1 || t2)
							csvw.addLaunch(cnt, lt, l);
						printSoundingInformation(cnt, s);
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
	if (final_prompt)
		system("pause");
	return 0;
}

