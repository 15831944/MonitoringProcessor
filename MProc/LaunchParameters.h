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
	/*friend bool operator<(const LaunchTime& l, const LaunchTime& r)
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
	}*/
	friend bool operator<(const LaunchTime& l, const LaunchTime& r)
	{
		int m1 = l.tm_min + l.tm_hour * 60 + l.tm_day * 24 * 60 + minutesInMonth(l.tm_mon, l.tm_year);
		int m2 = r.tm_min + r.tm_hour * 60 + r.tm_day * 24 * 60 + minutesInMonth(r.tm_mon, r.tm_year);
		return (m1<m2);
	}
	friend bool operator>(const LaunchTime& l, const LaunchTime& r)
	{
		int m1 = l.tm_min + l.tm_hour * 60 + l.tm_day * 24 * 60 + minutesInMonth(l.tm_mon, l.tm_year);
		int m2 = r.tm_min + r.tm_hour * 60 + r.tm_day * 24 * 60 + minutesInMonth(r.tm_mon, r.tm_year);
		return (m1>m2);
	}
	friend bool operator==(const LaunchTime& l, const LaunchTime& r)
	{
		int m1 = l.tm_min + l.tm_hour * 60 + l.tm_day * 24 * 60 + minutesInMonth(l.tm_mon, l.tm_year);
		int m2 = r.tm_min + r.tm_hour * 60 + r.tm_day * 24 * 60 + minutesInMonth(r.tm_mon, r.tm_year);
		return (m1==m2);
	}
	bool isNightLaunch()
	{
		if (tm_hour == 23)
		{
			return true;
		}
		return false;
	}
	bool isDayLaunch()
	{
		if (tm_hour == 11)
		{
			return true;
		}
		return false;
	}
	//process launchTime to parameters
	void parseLaunchTime()
	{
		size_t dot = launchTime.find(".");
		if (dot != std::string::npos)
		{
			tm_hour = atoi(launchTime.substr(0, dot).c_str());
			tm_min = atoi(launchTime.substr(dot + 1, launchTime.length()).c_str());
		}
		else
		{
			tm_hour = atoi(launchTime.substr(0, 2).c_str());
			tm_min = atoi(launchTime.substr(launchTime.length()-2, 2).c_str());
		}
		
	}
	int tm_min;   // minutes of hour from 0 to 59
	int tm_hour;  // hours of day from 0 to 24
	int tm_day;  // day of month from 1 to 31
	int tm_mon;   // month of year from 0 to 11
	int tm_year;  // year since 1900
	string launchTime;
	static int minutesInMonth(int month, int y)
	{
		int numberOfDays;
		if (month == 4 || month == 6 || month == 9 || month == 11)
			numberOfDays = 30;
		else if (month == 2)
		{
			bool isLeapYear = (y % 4 == 0 && y % 100 != 0) || (y % 4 == 0);
			if (isLeapYear)
				numberOfDays = 29;
			else
				numberOfDays = 28;
		}
		else
			numberOfDays = 31;
		return numberOfDays * 60 * 24;
	}
private:
	
};


typedef string StringLaunchTime;
/*class StringLaunchTime
{
public:
	string launchTime;
};*/

struct LaunchParameters
{
	string radarCode;
	string filesAvail;
	vector<string> strparams;
	vector<double> params;
	int longitude;
};

#endif