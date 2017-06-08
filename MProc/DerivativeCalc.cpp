#include "DerivativeCalc.h"

float DerivativeCalculator::derivative()
{
	float der = 0;
	current_error = 0;
	for (int i = 0; i != ctime - time200-1; i++)
	{
		der = data[i + 1] - data[i];
		if (abs(der) > thrValue)
		{
			current_error++;
			mark--;;
			if (mark < 0)
				mark = 0;
		}
	}
	
	slices_errors.push_back(current_error);
	
	return der;
}

int DerivativeCalculator::numErrors()
{
	int sum=0;
	for (auto i : slices_errors)
	{
		sum += i;
	}
	return sum;
}

void DerivativeCalculator::customClear()
{
	slices_errors.clear();
}