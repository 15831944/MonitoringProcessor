// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"
#include "CSVWorker.h"

using namespace std;

int station_index, year, month, day;
int morning_min;
int night_min;
string s_station_index, s_year, s_month;
string curdir="";
string outdir="";

int daysInMonth(int month)
{
	int numberOfDays;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		numberOfDays = 30;
	else if (month == 2)
	{
		bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (isLeapYear)
			numberOfDays = 29;
		else
			numberOfDays = 28;
	}
	else
		numberOfDays = 31;
	return numberOfDays;
}

string readFormat(zip_file &file, string base, int dayornight, string format)
{
	string data;
	string filename;
	char prefix[50];
	for (int i = 30; i != 60; i++)
	{
			if (!dayornight)
			{
				sprintf_s(prefix, "-11.%02d%s", i, format.c_str());
				morning_min = i;
			}
			else
			{
				sprintf_s(prefix, "-23.%02d%s", i, format.c_str());
				night_min = i;
			}
				
			filename = base + string(prefix);
			try
			{
				data = file.read(filename);
				return data;
			}
			catch (...)
			{

			}
	}
	//Vector-M radar
	for (int i = 30; i != 60; i++)
	{
		if (!dayornight)
		{
			sprintf_s(prefix, "-11%02d%s", i, format.c_str());
			morning_min = i;
		}
		else
		{
			sprintf_s(prefix, "-23%02d%s", i, format.c_str());
			night_min = i;
		}

		filename = base + string(prefix);
		try
		{
			data = file.read(filename);
			return data;
		}
		catch (...)
		{

		}
	}
	throw 1;
}

int strToInt(string myString)
{
	istringstream buffer(myString);
	int value;
	buffer >> value;
	return value;
}

void recognizeToken(string arg)
{
	if (arg.find('Y') != string::npos)
	{
		s_year = arg.substr(arg.find('Y')+2,arg.length()-2);
		year = strToInt(s_year);
	}
	if (arg.find('M') != string::npos)
	{
		s_month = arg.substr(arg.find('M') + 2, arg.length() - 2);
		month = strToInt(s_month);
	}
	if (arg.find('I') != string::npos)
	{
		s_station_index = arg.substr(arg.find('I') + 2, arg.length() - 2);
		station_index = strToInt(s_station_index);
	}
	if (arg.find('R') != string::npos)
	{
		outdir = arg.substr(arg.find('S') + 2, arg.length() - 2);
	}
	if (arg.find('S') != string::npos)
	{
		curdir = arg.substr(arg.find('S') + 2, arg.length() - 2);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	int mode=1;
	int radar = 1;
	//TODO ADD pre-load CSV
	CSVWorker csvw;
	day = 1;

	if(argc <= 1)
    {
        std::cout << "usage: " << argv[0] << " filename" << std::endl;
		std::cout << "usage: " << argv[0] << " yyyy iiiii mm" << std::endl;
        return 0;
    }

	string infile;
	string outfile;
	
	stringstream ss;
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
		/*ss << argv[1] << '/' << argv[2] << '/' << argv[2] << '-' << argv[1] << argv[3] << "A.zip";
		infile = ss.str();
		oss = std::istringstream(argv[1]);
		oss >> year;
		oss.clear();
		oss = std::istringstream(argv[3]);
		oss >> month;
		oss.clear();
		oss = std::istringstream(argv[2]);
		oss >> station_index;
		oss.clear();*/

		for (i = 1; i != argc; i++)
			recognizeToken(string(argv[i]));

		ss << s_year << '/' << s_station_index << '/' << s_station_index << '-' << s_year << s_month;// << radar1[radar] << ".zip";
		infile = ss.str();
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

	
	//str_d << day << '/' << day << '.' << month << '.' << year << "-11.30";
	//ss << s_station_index << '-' << s_year << s_month << radar1[radar] << '/' << month;
	//ss << radar2[radar] << '/' << s_month << '/' << day << '.' << month << '.' << year;//<< "-11.30";
	//string morningstr = ss.str() + "-11.30";
	//string nightstr = ss.str() + "-23.35";

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
		csvw.readCSV(outfile);
		for (radar = 0; radar != NUMRADARS; radar++)
		{
			try
			{
				zip_file file(infile + radar2[radar]);
				string base = file.get_first_filename();
				base = base.substr(0, base.find_last_of('/'));
				ss << base << '/' << day << '.' << month << '.' << year;
				//base = ss.str();

				for (day = 1; day <= daysInMonth(month); day++)
				{
					Sounding s;
					s.setRAWDataIdentifier(raw_radar[radar]);
					ss.clear();
					ss = stringstream();

					if (radar != 2)
					{
						ss << base << '/' << day << '.' << month << '.' << year;// << "-11.30";
					}
					else
					{
						char date[10];
						sprintf_s(date, "%04d%02d%02d", year, month, day);
						ss << base << '/' << string(date);
					}

					morningstr = ss.str();
					nightstr = morningstr;
					/*ss.clear();
					ss = stringstream();
					ss << base << '/' << day << '.' << month << '.' << year;// << "-23.35";
					nightstr = ss.str();*/
					
					//file.printdir();
					//file.read("25703-201609R/9Œ/9.9.2016-11.30.RAW");
					cout << morningstr << endl;
					for (i = 0; i != N; i++)
					{
						s.setDayOrNight(0);
						try
						{
							//file.read("25703-201609R/9.9.2016-11.30.RAW");
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
					s.setDayOrNight(0);
					s.processRAWFile();
					//yyyy.mm.dd hh:mm
					LaunchTime lt;
					lt.tm_year = year;
					lt.tm_mon = month;
					lt.tm_day = day;
					lt.tm_hour = 11;
					lt.tm_min = 30;// morning_min;
					LaunchParameters l;
					l.radarCode = radar1[radar];
					int t1 = s.getRAWSoundingTime();
					int t2 = s.getSoundingTime();
					l.params.push_back((double)s.getRAWSoundingTime());
					l.params.push_back((double)s.getMaxAltitude());

					if (t1||t2)
						csvw.addLaunch(0, lt, l);

					cout << "-----------------------------------\n SUMMARY INFORMATION MORNING" << endl;
					cout << "Formats " << s.checkFormats() << " of 14" << endl;
					cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
					cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
					cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
					s.setDayOrNight(1);
					s.processRAWFile();

					LaunchTime lt2;
					lt2.tm_year = year;
					lt2.tm_mon = month;
					lt2.tm_day = day;
					lt2.tm_hour = 23;
					lt2.tm_min = 30;//night_min;
					l = LaunchParameters();
					l.radarCode = radar1[radar];
					t1 = s.getRAWSoundingTime();
					t2 = s.getSoundingTime();
					l.params.push_back((double)s.getRAWSoundingTime());
					l.params.push_back((double)s.getMaxAltitude());
					
					if (t1 || t2)
						csvw.addLaunch(1, lt2, l);

					cout << "-----------------------------------\n SUMMARY INFORMATION NIGHT" << endl;
					cout << "Formats " << s.checkFormats() << " of 14" << endl;
					cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
					cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
					cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
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

