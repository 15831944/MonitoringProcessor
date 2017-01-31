#include "CSVWorker.h"

void CSVWorker::addLaunch(int dayornight, LaunchTime lTime, LaunchParameters lParams)
{
	if (!dayornight)
		monthDataDay[lTime] = lParams;
	else
		monthDataNight[lTime] = lParams;
}

void CSVWorker::readCSV(string filename)
{

}

void CSVWorker::writeCSV(string filename)
{
	csv::ofstream os(filename);
	os.set_delimiter(',', "$$");
	if (os.is_open())
	{
		map<LaunchTime, LaunchParameters>::iterator i;
		map<LaunchTime, LaunchParameters>::iterator it2 = monthDataNight.begin();
		for (i = monthDataDay.begin(); i != monthDataDay.end(); i++)
		{
			LaunchTime lt = (*i).first;
			string dateStr = lt.getAsString();
			os << dateStr << (*i).second.radarCode;
			for (auto it : (*i).second.params)
			{
				os << it;
			}
			os << NEWLINE;
			lt = (*it2).first;
			dateStr = lt.getAsString();
			os << dateStr << (*i).second.radarCode;
			for (auto it : (*it2).second.params)
			{
				os << it;
			}
			os << NEWLINE;
			it2++;
		}
	}
	os.flush();
}
