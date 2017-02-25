#include "MarkGenerator.h"

float MarkGen::getQualityMark()
{
	float sum=0;
	for (int i = 0; i != MARKS_NUMBER; i++)
	{
		sum += marks[i] * mark_weights[i];
	}
	sum /= MARKS_NUMBER;

	return sum;
}