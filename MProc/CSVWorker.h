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
	void addLaunch(LaunchTime lTime, LaunchParameters lParams);
	bool hasLaunch(LaunchTime lTime);
	LaunchParameters getLaunch(int dayornight, LaunchTime lTime);
	void writeLaunch(csv::ofstream &os, LaunchTime lt, LaunchParameters lp);
	void readCSV(string filename);
	void writeCSV(string filename);
	// Так как схема с 2 пусками за день не оправдала себя, 
	// Вводится новый режим функционирования.
	// Тип - 1 - новый.
	// Тип - 0 - старый.
	void setOperationType(int t);
private:
	string toLower(string data);
	int type;
	LaunchTime parseTime(string datetime);
	map<LaunchTime, LaunchParameters> monthDataDay;
	map<LaunchTime, LaunchParameters> monthDataNight;
	map<LaunchTime, LaunchParameters> monthData;
};

#endif