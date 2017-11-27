#ifndef TAE3_H
#define TAE3_H

#include "stdafx.h"

class TAE3
{
public:
	TAE3();
	~TAE3(){};
	void addString(string str);
	void addString_legacy(string str);
	void compute();
	void addSunString(string sun);
	void addTempErrorString(string terror);
	void addHumErrorString(string humerror);
	inline float getAverageWindDirection()
	{
		return avD;
	}
	inline float getAverageWindSpeed()
	{
		return avV;
	}
	inline float getHumError()
	{
		return herr;
	}
	inline float getTempError()
	{
		return terr;
	}
	inline float getSunHeight()
	{
		return sunh;
	}
private:
	float sumcos;
	float sumsin;
	float avV;
	float avD;
	float sunh, terr, herr;
	int dcnt;
};


#endif