#ifndef LAUNCHPARAMETERS_H
#define LAUNCHPARAMETERS_H

#include "stdafx.h"

using namespace std;

class LaunchTime
{
public:
	LaunchTime(){};
	~LaunchTime(){};
	string getAsString()
	{ 
		//yyyy.mm.dd hh:mm
		char stri[20];
		sprintf_s(stri, "%04d.%02d.%02d %02d:%02d", tm_year, tm_mon,
			tm_day, tm_hour, tm_min);
		return string(stri);
	}
	friend bool operator<(const LaunchTime& l, const LaunchTime& r)
	{
		return (l.tm_day < r.tm_day);
	}
	friend bool operator>(const LaunchTime& l, const LaunchTime& r)
	{
		return (l.tm_day > r.tm_day);
	}
	friend bool operator==(const LaunchTime& l, const LaunchTime& r)
	{
		return (l.tm_day == r.tm_day);
	}
	int tm_min;   // minutes of hour from 0 to 59
	int tm_hour;  // hours of day from 0 to 24
	int tm_day;  // day of month from 1 to 31
	int tm_mon;   // month of year from 0 to 11
	int tm_year;  // year since 1900
};

struct LaunchParameters
{
	string radarCode;
	string filesAvail;
	vector<string> strparams;
	vector<double> params;
};

#endif