#include <time.h>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;

int checkLocalTime(tm startTime, int StationLongitude)
{
	int hourZone = (int)(((float)StationLongitude + 7.5f) / 15.0f) + 1;
	//startTime.tm_year -= 1900;
	time_t delta = mktime(&startTime)-hourZone*3600;
	tm* deltaTime = localtime(&delta);
	if ((deltaTime->tm_hour == 22) || (deltaTime->tm_hour == 23) || (deltaTime->tm_hour == 0))
	{
		return 1;
	}
	else 
	{
		if ((deltaTime->tm_hour == 10) || (deltaTime->tm_hour == 11) || (deltaTime->tm_hour == 12))
		{
			return 2;
		}
	}
	return 0;
}

tm* getGMTFromLocal(tm startTime, int StationLongitude)
{
	int hourZone = (int)(((float)StationLongitude + 7.5f) / 15.0f) + 1;
	//startTime.tm_year -= 1900;
	time_t delta = mktime(&startTime) - hourZone * 3600;
	tm* deltaTime = localtime(&delta);
	return deltaTime;
}

