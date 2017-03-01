#include "DispersionCalc.h"
#include "stdafx.h"

void DispersionCalculator::dataInput(int time, float d)
{
	if (ctime == -1)
	{
		data[0] = d;
		ctime = 0;
		return;
	}
	float olddata = data[ctime - time200];
	int oldtime = ctime;
	do
	{
		ctime++;
		data[ctime-time200] = olddata + (d - olddata)*((float)(ctime - oldtime) / (float)(time - oldtime));
		if (ctime > time200+198)
		{
			dispersion();
			time200 = ctime+1;
		}
	} while (ctime < time);
}

float DispersionCalculator::dispersion()
{
	float d=0;
	float m = mean();
	for (int i = 0; i != ctime - time200; i++)
	{
		d += pow((data[i] - m), 2);
	}
	d /= (ctime - ltime);
	ltime = ctime;
	if (d > thrValue)
	{
		mark--;
		if (mark < 0)
			mark = 0;
	}
	return d;
}

float DispersionCalculator::mean()
{
	float m=0;
	for (int i = ltime; i != ctime; i++)
	{
		m += data[i];
	}
	return m / (ctime - ltime);
}