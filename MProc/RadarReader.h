#include "stdafx.h"
#include "zip_file.hpp"
#include "CSVWorker.h"
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
	void setCSVWorker(CSVWorker* csvw);
	void setRadarNumber(int radar);
	void setSettings(vector<bool> stngs);
private:
	string radar_readFormat(zip_file &file, string base, int dayornight, string format);
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