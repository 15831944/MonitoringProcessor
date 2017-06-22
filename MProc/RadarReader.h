#include "stdafx.h"
#include "zip_file.hpp"
#include "CSVWorker.h"
#include "Sounding.h"
#ifndef RADARREADER_H
#define RADARREADER_H

class RadarReader
{
public:
	RadarReader();
	~RadarReader();
	void init(string radarPrefix, string radarZIP, string rawPrefix, string radarName);
	void initFromFile(string infile, string infile2);
	void processMonth(int m,int y);
	void processMonthInd(int m, int y); //ќбработчик данных за мес€ц без прив€зки к 11:30 и 22:30
	void setCSVWorker(CSVWorker* csvw);
	void setRadarNumber(int radar);
	void setSettings(vector<bool> stngs);
private:
	string radar_readFormat(zip_file &file, string base, int dayornight, string format);
	string radar_readFormat(zip_file &file, string base);
	void radar_processFormats(Sounding &s);
	void radar_processFormats(Sounding &s, vector<string> names);
	void radar_processLaunch(Sounding &s, LaunchTime &lt1, LaunchTime &lt2, int cnt);
	void radar_processLaunch(Sounding &s);
	void radar_makeDayNightStrings();
	vector<string> radar_makeTimeStrings(vector<string> names);
	map<string, string> radar_makeTimeFormatStrings(vector<string> names);
	vector<string> radar_makeFormatStrings(vector<string> names);
	string separateFormatFromPath(string fullname);
	string removeFormatFromPath(string fullname);
	LaunchTime getLaunchTimeFromString(string filename);
	string mRadarName;
	string mRadarPrefix;
	string mRadarZip;
	string mRawPrefix;
	string test_pr, base;
	string data;
	string datam[N];
	string datan[N];
	string error_str;
	string morningstr;
	string nightstr;
	CSVWorker* mCsvw;
	int year, month, day;
	int mRadarNumber;
	int morning_min;
	int night_min;
	zip_file file;
	stringstream ss;
	vector<bool> mSettings;
};

#endif