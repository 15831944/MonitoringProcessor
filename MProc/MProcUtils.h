#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"
#include "CSVWorker.h"
#include "INIReader.h"
#include "MarkGenerator.h"

#ifndef MPROCUTILS_H
#define MPROCUTILS_H

int station_index, year, month, day;
int morning_min;
int night_min;
string s_station_index, s_year, s_month;
string curdir = "";
string outdir = "";

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

#endif
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

inline bool fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}