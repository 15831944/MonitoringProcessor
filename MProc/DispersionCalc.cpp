#include "DispersionCalc.h"
#include "stdafx.h"

void DispersionCalculator::dataInput(int time, float d)
{
	if (ctime == -1)
	{
		mark = 10;
		data[0] = d;
		ctime = 0;
		olddata = d;
		return;
	}
	//float olddata = data[ctime - time200];
	int oldtime = ctime;
	if (time == oldtime)
		return;
	do
	{
		ctime++;
		data[ctime-time200] = olddata + (d - olddata)*((float)(ctime - oldtime) / (float)(time - oldtime));
		olddata = data[ctime - time200];
		if (ctime > time200+198)
		{
			dispersion_window();
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

float DispersionCalculator::dispersion_window()
{
	float d = 0;
	float m = mean();sdfsdfsdfsdf
	float window = 20;
	int kr = (int)((ctime - time200) / window);
	for (int i = 0; i != kr-1; i++)
	{
		d += dispersion((float)i*window, ((float)i + 1)*window);
	}
	d /= (ctime - time200) / window;
	ltime = ctime;
	
	if ((d > thrValue) && (ctime>200))
	{
		std::cout << "Dispcalc:" << d << ' ' << ctime << ' ' << data[0]<<std::endl;
		mark--;
		if (mark < 0)
			mark = 0;
	}
	return d;
}

float DispersionCalculator::dispersion(int start, int end)
{
	double d = 0;
	float m = mean(start, end);
	for (int i = start; i != end; i++)
	{
		d += pow((data[i] - m), 2);
	}
	d /= (end-start);
	return (float)sqrt(d);
}

float DispersionCalculator::mean()
{
	return mean(ltime + 1 - time200, ctime + 1 - time200);
}

float DispersionCalculator::mean(int start, int end)
{
	float m = 0;
	for (int i = start; i != end; i++)
	{
		m += data[i];
	}
	return m / (end - start);
}
