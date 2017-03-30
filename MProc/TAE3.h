#ifndef TAE3_H
#define TAE3_H

#include "stdafx.h"

class TAE3
{
public:
	TAE3();
	~TAE3(){};
	void addString(string str);
	void compute();
	inline float getAverageWindDirection()
	{
		return avD;
	}
	inline float getAverageWindSpeed()
	{
		return avV;
	}
private:
	float sumcos;
	float sumsin;
	float avV;
	float avD;
	int dcnt;
};


#endif