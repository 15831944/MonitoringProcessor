#ifndef SOUNDING_H
#define SOUNDING_H

#include "stdafx.h"
#include "RAWData.h"

class Sounding
{
public:
	Sounding()
	{
		mRawData = ".RAW";
	};
	~Sounding()
	{
	};
	
	void setRAWDataIdentifier(string rdata);
	void setDayOrNight(char dayornight);
	void addData(string data, string format);
	bool hasFormat(string format);
	string getFormat(string format);
	int checkFormats();
	string getFormatsTelegram();
	int getSoundingTime();
	int getRAWSoundingTime();
	int getMaxAltitude();
	int getKN04Code();
	void processRAWFile();
	void processKN04File();
private:
	RAWData rDaM;  /*morning*/
	RAWData rDaN; /* night */
	int dayornight;
	string mRawData;
	string getRAWfile();
	string getCRDfile();
	string getKN04file();
	int KN04Code1;
	int KN04Code2;
	map<string, string> mDataDay;
	map<string, string> mDataNight;
};

#endif