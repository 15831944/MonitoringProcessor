#include <time.h>

int checkLocalTime(tm startTime, int StationLongitude)
{
	int hourZone = (int)(((float)StationLongitude + 7.5f) / 15.0f);
	startTime.tm_hour -= hourZone;
	if (startTime.tm_hour < 0)
	{
		startTime.tm_mday -= 1;
		startTime.tm_hour += 24;
		if (startTime.tm_mday < 0)
		{
			startTime.tm_mon -= 1;
			startTime.tm_mday += 
}