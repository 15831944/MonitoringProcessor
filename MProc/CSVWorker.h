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
	bool hasLaunch(LaunchTime lTime);
	LaunchParameters getLaunch(int dayornight, LaunchTime lTime);
	void writeLaunch(csv::ofstream &os, LaunchTime lt, LaunchParameters lp);
	void readCSV(string filename);
	void writeCSV(string filename);
private:
	LaunchTime parseTime(string datetime);
	map<LaunchTime, LaunchParameters> monthDataDay;
	map<LaunchTime, LaunchParameters> monthDataNight;
};

#endif