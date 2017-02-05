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
	csv::ifstream is(filename);
	is.set_delimiter(',', "$$");
	if (is.is_open())
	{
		while (is.read_line())
		{
			LaunchTime lt;
			LaunchParameters lp;
			string datetime,radarcode;
			csv::sep colon(':', "<colon>");
			csv::sep comma(',', "<comma>");
			csv::sep space(' ', "<space>");
			csv::sep point('.', "<point>");
			double data;
			is >> datetime >> lp.radarCode;
			for (unsigned char i = 0; i != NUMPARAMETERS; i++)
			{
				is >> data;
				lp.params.push_back(data);
			}
			try
			{
				lt = parseTime(datetime);
			}
			catch (...){}
			if (lt.tm_hour > 10 && lt.tm_hour < 20)
				monthDataDay[lt] = lp;
			else
				monthDataNight[lt] = lp;
		}
	}
}

LaunchTime CSVWorker::parseTime(string datetime)
{
	LaunchTime lt;
	sscanf_s(datetime.c_str(), "%04d.%02d.%02d %02d:%02d", &lt.tm_year, &lt.tm_mon, &lt.tm_day, &lt.tm_hour, &lt.tm_min);
	return lt;
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
			os << dateStr << (*it2).second.radarCode;
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
