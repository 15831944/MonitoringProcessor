#include "stdafx.h"
#include "RadarUtils.h"
#include "zip_file.hpp"

#ifndef RADARREADER_H
#define RADARREADER_H

class RadarReader
{
public:
	RadarReader();
	~RadarReader();
	void init(string radarPrefix, string radarZIP, string rawPrefix, string radarName);
	void initFromFile(string infile, string infile2);
	void processMonth();

private:
	string mRadarName;
	string mRadarPrefix;
	string mRadarZip;
	string mRawPrefix;
	string test_pr, base;
	zip_file file;
	stringstream ss;
};

#endif