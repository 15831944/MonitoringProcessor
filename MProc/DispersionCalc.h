#ifndef DISPCALC_H
#define DISPCALC_H

#include "stdafx.h"

class DispersionCalculator
{
public:
	DispersionCalculator()
	{
		datacnt = 0;
		ctime = 0;
		ltime = 0;
		thrValue = 10.0f;
		mark = 10;
		time200 = 0;
	};
	~DispersionCalculator()
	{
	};
	inline void setThresholdValue(float v){
		thrValue = v;
	}
	void dataInput(int time, float d);
	float dispersion();
	float mean();
	inline int getDispersionMark(){ return mark; }
private:
	int mark;
	float thrValue;
	float data[300];
	int datacnt;
	int ctime,ltime;
	int time200;
};

#endif