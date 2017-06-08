#include "Calculator.h"
#include <vector>

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
		current_error = 0;
	}
	~DerivativeCalculator()
	{
	}
	float calculate_window()
	{
		return derivative();
	};

	void customClear();

	int numErrors();
	float derivative();
	std::vector<int> slices_errors;
	int current_error;


};

#endif