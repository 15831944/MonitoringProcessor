#ifndef SOUNDING_H
#define SOUNDING_H

#include "stdafx.h"
#include "RAWData.h"
#include "TAE3.h"
#include "DayFormats.h"

//Класс со всеми зондированиями за день.

class Sounding
{
public:
	Sounding()
	{
		mRawData = ".RAW";
		infozond1 = infozond2 = 0;
		type = 0;
		longitude = 0;
	};
	~Sounding()
	{

	};
	
	// Так как схема с 2 пусками за день не оправдала себя, 
	// Вводится новый режим функционирования.
	// Тип - 1 - новый.
	// Тип - 0 - старый.
	void setOperationType(int t);
	void setRAWDataIdentifier(string rdata);
	void setDayOrNight(char dayornight);
	void setNewTime(LaunchTime newlTime);
	void addData(string data, string format);
	bool hasFormat(string format);
	string getFormat(string format);
	int checkFormats();
	string getFormatsTelegram();
	int getSoundingTime();
	int getRAWSoundingTime();
	int getMaxAltitude();
	int getMaxDistance();
	int getKN04Code();
	int getInfoZondType();
	int getNumSpikes();
	float getMinElevation();
	float getGroundTemperature();
	float getMinTemperature();
	float getMinTemperature(float H);
	float getAverageWindDirection();
	float getAverageWindSpeed();
	float getSunHeight();
	float getLandErrorHum();
	float getLandErrorTemp();
	int getAlt10Elevation();
	void processRAWFile();
	void processKN04File();
	void processTAE3File();
	void processINFOFile();
	int getLongitude();
	map<StringLaunchTime, DayFormats*> mAllData; //для нового режима функционирования
private:
	int longitude;
	int type;
	RAWData rDaM;  /*morning*/
	RAWData rDaN; /* night */
	TAE3 tDaM;  /*morning*/
	TAE3 tDaN; /* night */
	int dayornight;
	string mRawData;
	string getRAWfile();
	string getCRDfile();
	string getKN04file();
	string getINFOfile();
	string getTAE3file();
	int KN04Code1;
	int KN04Code2;
	int infozond1;
	int infozond2;
	map<string, string> mDataDay;
	map<string, string> mDataNight;
	StringLaunchTime curLaunchTime; //для нового режима функционирования
};

#endif