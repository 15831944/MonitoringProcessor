#ifndef SOUNDING_H
#define SOUNDING_H

#include "stdafx.h"
#include "RAWData.h"

class Sounding
{
public:
	Sounding()
	{
	};
	~Sounding()
	{
	};
	
	void setDayOrNight(char dayornight);
	void addData(string data, string format);
	bool hasFormat(string format);
	string getFormat(string format);
	int checkFormats();
	int getSoundingTime();
	int getRAWSoundingTime();
	int getMaxAltitude();
	void processRAWFile();
private:
	RAWData rDaM;  /*morning*/
	RAWData rDaN; /* night */
	int dayornight;
	string getRAWfile();
	string getCRDfile();
	map<string, string> mDataDay;
	map<string, string> mDataNight;
};

#endif