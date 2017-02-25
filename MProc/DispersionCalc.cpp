#include "DispersionCalc.h"

void DispersionCalculator::dataInput(int time, float d)
{
	if (ctime == 0)
	{
		data[0] = d;
	}
	float olddata = data[ctime];
	int oldtime = ctime;
	while (ctime != time)
	{
		data[ctime-time200] = olddata + (d - olddata)*((float)(ctime - oldtime) / (float)(time - oldtime));
		ctime++;
		if (ctime > 200)
		{
			dispersion();
			time200 = ctime;
		}
	}
}

float DispersionCalculator::dispersion()
{
	float d=0;
	float m = mean();
	for (int i = ltime; i != ctime; i++)
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