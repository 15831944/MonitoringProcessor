#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"
#include "CSVWorker.h"
#include "INIReader.h"
//#include "RadarUtils.h"

#ifndef MPROCUTILS_H
#define MPROCUTILS_H

int station_index, year, month, day;
int morning_hour;
int night_hour;
int morning_min;
int night_min;
string s_station_index, s_year, s_month;
string curdir = "";
string outdir = "";
string error_str;
string infile, infile2;
string outfile;
CSVWorker* csvw;
bool final_prompt;

vector<bool> settings;

//#define FIND_ALL_FILES

/*int daysInMonth(int month,int y)
{
	int numberOfDays;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		numberOfDays = 30;
	else if (month == 2)
	{
		bool isLeapYear = (y % 4 == 0 && y % 100 != 0) || (year % 4 == 0);
		if (isLeapYear)
			numberOfDays = 29;
		else
			numberOfDays = 28;
	}
	else
		numberOfDays = 31;
	return numberOfDays;
}*/

string try_readFormat(zip_file &file, string base, int dayornight, string format, string formattedString, int hour, int minute)
{
	string data="";
	string filename;
	char prefix[50];

	sprintf_s(prefix, formattedString.c_str(), hour,minute, format.c_str());
	filename = base + string(prefix);
	try
	{
		data = file.read(filename);
		if (!dayornight)
		{
			morning_hour = hour;
			morning_min = minute;
			if (hour != 11)
			{
				error_str = "?";
			}
			else
			{
				error_str = "";
			}
		}
		else
		{
			if (hour != 23)
			{
				error_str = "?";
			}
			else
			{
				error_str = "";
			}
		}
		return data;
	}
	catch (...)
	{
		/*(if (!dayornight)
		{
			morning_hour = 0;
			morning_min = 0;
		}*/
	}

	return data;
}

string readFormat(zip_file &file, string base, int dayornight, string format)
{
	string data;
	string filename;
	char prefix[50];
#ifdef FIND_ALL_FILES
	if (!dayornight)
	{
		for (int j = 0; j != 24; j++)
		{
			for (int i = 0; i != 60; i++)
			{
				data = try_readFormat(file, base, dayornight, format, "-%02d.%02d%s", j, i);
				if (!data.empty())
					return data;
				data = try_readFormat(file, base, dayornight, format, "-%02d%02d%s", j, i);
				if (!data.empty())
					return data;
			}
		}
	}
	else
	{
		if (morning_hour || morning_min)
		{
			//WARNING!!!
			// Если пуск длился менее часа то может возникнуть ошибка, но 
			// Такое очень редко бывает, наверное даже никогда. 
			for (int j = morning_hour+1; j != 24; j++)
			{
				for (int i = 0; i != 60; i++)
				{
					data = try_readFormat(file, base, dayornight, format, "-%02d.%02d%s", j, i);
					if (!data.empty())
						return data;
					data = try_readFormat(file, base, dayornight, format, "-%02d%02d%s", j, i);
					if (!data.empty())
						return data;
				}
			}
		}
		
	}
#else
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
#endif

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
		s_year = arg.substr(arg.find('Y') + 2, arg.length() - 2);
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

void readSetupFile()
{
	stringstream ss;
	int i;
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
		final_prompt = reader.GetBoolean("settings", "prompt", false);

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

		for (i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			ss << "param" << i;
			mg.setDerivativeCalcThreshold(i, reader.GetReal("DerivativeCalculator", ss.str(), 30.0f));
			ss.clear();
			ss = stringstream();
		}

	}
}

bool printUsageStrings(int argc, _TCHAR* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " filename" << std::endl;
		std::cout << "usage: " << argv[0] << " Y=yyyy I=iiiii M=mm" << std::endl;
		return 0;
	}
	return 1;
}

void readCSVFile()
{
	stringstream ss;
	try
	{
		csvw->readCSV(outfile);
	}
	catch (...)
	{
		ss << "ERROR Reading input file" << endl;
	}
}

void processInputParameters(int argc, _TCHAR* argv[])
{
	std::istringstream oss;
	stringstream ss;
	int i;
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
}

void makeInOutFilenames()
{
	stringstream ss;
	ss.clear();
	ss = stringstream();
	ss << s_station_index << '-' << s_year << s_month << ".csv";
	outfile = outdir + ss.str();
	infile = curdir + infile;
}

#endif

