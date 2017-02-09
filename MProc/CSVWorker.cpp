#include "CSVWorker.h"

void CSVWorker::addLaunch(int dayornight, LaunchTime lTime, LaunchParameters lParams)
{
	//TODO check LaunchTime exists +
	//TODO check already loaded formats for any LaunchTime

	if (hasLaunch(lTime))
	{
		LaunchParameters lp = monthDataDay[lTime];
	}
	

	if (!dayornight)
		monthDataDay[lTime] = lParams;
	else
		monthDataNight[lTime] = lParams;
}

bool CSVWorker::hasLaunch(LaunchTime lTime)
{
	if (monthDataDay[lTime].radarCode.length() > 0)
		return true;
	if (monthDataNight[lTime].radarCode.length() > 0)
		return true;
	return false;
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
			string s_data;
			is >> datetime >> lp.radarCode;
			for (unsigned char i = 0; i != NUMPARAMETERS_STR; i++)
			{
				is >> s_data;
				lp.strparams.push_back(s_data);
			}
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

LaunchParameters CSVWorker::getLaunch(int dayornight, LaunchTime lTime)
{
	//DO NOT USE!!!!
	LaunchParameters lp;
	map<LaunchTime, LaunchParameters>::iterator i;
	for (i = monthDataDay.begin(); i != monthDataDay.end(); i++)
	{
		//if ()
	}
	return lp;
}

void CSVWorker::writeLaunch(csv::ofstream &os, LaunchTime lt, LaunchParameters lp)
{
	string dateStr = lt.getAsString();
	os << dateStr << lp.radarCode;
	for (auto it : lp.strparams)
	{
		os << it;
	}
	for (auto it : lp.params)
	{
		os << it;
	}
	os << NEWLINE;
}

void CSVWorker::writeCSV(string filename)
{
	csv::ofstream os(filename);
	os.set_delimiter(',', "$$");
	if (os.is_open())
	{
		map<LaunchTime, LaunchParameters>::iterator it1 = monthDataDay.begin();
		map<LaunchTime, LaunchParameters>::iterator it2 = monthDataNight.begin();
		
		while ((it1 != monthDataDay.end()) && (it2 != monthDataNight.end()))
		{
			if (((*it1).first < (*it2).first) || ((*it1).first == (*it2).first))
			{
				writeLaunch(os, (*it1).first, (*it1).second);
				writeLaunch(os, (*it2).first, (*it2).second);
				it1++;
				it2++;
				continue;
			}
			if (((*it1).first >(*it2).first))
			{
				writeLaunch(os, (*it2).first, (*it2).second);
				writeLaunch(os, (*it1).first, (*it1).second);
				it1++;
				it2++;
			}
		}
		if ((it1 == monthDataDay.end()) && (it2 != monthDataNight.end()))
		{
			while (it2 != monthDataNight.end())
			{
				writeLaunch(os, (*it2).first, (*it2).second);
				it2++;
			}
		}
		if ((it1 != monthDataDay.end()) && (it2 == monthDataNight.end()))
		{
			while (it1 != monthDataDay.end())
			{
				writeLaunch(os, (*it1).first, (*it1).second);
				it1++;
			}
		}
		
		/*for (i = monthDataDay.begin(); i != monthDataDay.end(); i++)
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
		}*/
	}
	os.flush();
}
