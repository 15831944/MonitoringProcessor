#include "Calculator.h"

#ifndef DISPCALC_H
#define DISPCALC_H

class DispersionCalculator : public Calculator
{
public:
	DispersionCalculator()
	{
		datacnt = 0;
		ctime = -1;
		ltime = 0;
		thrValue = 10.0f;
		mark = 10;
		time200 = 0;
	}
	~DispersionCalculator()
	{
	}

	float calculate_window()
	{ 
		return dispersion_window(); 
	};
	float dispersion();
	float dispersion_window();
	float dispersion(int start, int end);
	float mean();
	float mean(int start,int end);
	inline int getDispersionMark(){ return mark; }
};

#endif