#include "Calculator.h"

void Calculator::dataInput(int time, float d)
{
	if (ctime == -1)
	{
		mark = 10;
		data[0] = d;
		ctime = 0;
		olddata = d;
		return;
	}
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
			calculate_window();
			time200 = ctime+1;
		}
	} while (ctime < time);
}