#include "MarkGenerator.h"

float MarkGen::getQualityMark()
{
	calcDispMark();
	float sum=0;
	int num_enabled = 0;
	for (int i = 0; i != MARKS_NUMBER; i++)
	{
		if (getMarkEnable(i))
		{
			sum += marks[i] * mark_weights[i];
			//cout << marks[i] << endl;
			num_enabled++;
		}
	}
	sum /= (float)num_enabled;

	return sum;
}

void MarkGen::calcDispMark()
{
	marks[2] = 0;
	for (int i = 0; i != DISPERSION_CALCULATORS; i++)
	{
		marks[2] += dispersion_calculators[i]->getDispersionMark();
	}
	marks[2] /= DISPERSION_CALCULATORS;
}