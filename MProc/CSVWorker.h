#ifndef CSVWORKER_H
#define CSVWORKER_H

#include "stdafx.h"

using namespace mini;

class CSVWorker
{
public:
	CSVWorker()
	{
	};
	~CSVWorker()
	{
	};
	void addLaunch(int dayornight,LaunchTime lTime, LaunchParameters lParams);
	void readCSV(string filename);
	void writeCSV(string filename);
private:
	map<LaunchTime, LaunchParameters> monthDataDay;
	map<LaunchTime, LaunchParameters> monthDataNight;
};

#endif