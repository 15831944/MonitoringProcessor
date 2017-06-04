#include "DerivativeCalc.h"

float DerivativeCalculator::derivative()
{
	float der = 0;
	for (int i = 0; i != ctime - time200-1; i++)
	{
		der = data[i + 1] - data[i];
	}
	
	if (der > thrValue)
	{
		mark--;
		if (mark < 0)
			mark = 0;
	}
	return der;
}