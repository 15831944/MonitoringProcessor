#include "Calculator.h"

#ifndef DERIVATIVECALC_H
#define DERIVATIVECALC_H

class DerivativeCalculator : public Calculator
{
public:
	DerivativeCalculator()
	{
		datacnt = 0;
		ctime = -1;
		ltime = 0;
		thrValue = 1000.0f;
		mark = 10;
		time200 = 0;
	}
	~DerivativeCalculator()
	{
	}
	float calculate_window()
	{
		return derivative();
	};
	float derivative();
};

#endif