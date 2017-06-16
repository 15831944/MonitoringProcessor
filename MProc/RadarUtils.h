#include "stdafx.h"
#include "Sounding.h"

#ifndef RADARUTILS_H
#define RADARUTILS_H


inline bool fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

inline void push_back_params(LaunchParameters &l, Sounding s, vector<bool> settings,string error_str)
{
	MarkGen& mg = MarkGen::Instance();
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
			case 12:
				l.params.push_back((double)s.getNumSpikes());
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
}

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

inline void printSoundingInformation(int cnt, Sounding s)
{
	cout << "-----------------------------------\n SUMMARY INFORMATION" << cnt << endl;
	cout << "Formats " << s.checkFormats() << " of 14" << endl;
	cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
	cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
	cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
	cout << "Max distance(RAW) " << s.getMaxDistance() << endl;
}

#endif