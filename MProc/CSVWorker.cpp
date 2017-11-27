#include "CSVWorker.h"
#include "LocalTimeCheck.h"

void CSVWorker::addLaunch(int dayornight, LaunchTime lTime, LaunchParameters lParams)
{
	//TODO check LaunchTime exists +
	//TODO check already loaded formats for any LaunchTime

	/*if (hasLaunch(lTime))
	{
		LaunchParameters lp = monthDataDay[lTime];
	}*/
	

	if (!dayornight)
		monthDataDay[lTime] = lParams;
	else
		monthDataNight[lTime] = lParams;
}

string CSVWorker::toLower(string dat)
{
	string data = dat;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

void CSVWorker::addLaunch(LaunchTime lTime, LaunchParameters lParams)
{
	LaunchTime lt = lTime;
	lt.parseLaunchTime();
	if (lt.isDayLaunch())
		monthDataDay[lt] = lParams;
	else if (lt.isNightLaunch())
		monthDataNight[lt] = lParams;
	else
	{
		//ѕуск отнести куда-либо сложно.
		//—юда ещЄ можно добавить проверку на местное врем€.
		tm startTime;
		memset(&startTime, 0, sizeof(tm));
		startTime.tm_year = lt.tm_year-1900;
		startTime.tm_mon = lt.tm_mon;
		startTime.tm_mday = lt.tm_day;
		startTime.tm_hour = lt.tm_hour;
		startTime.tm_min = lt.tm_min;
		int checkResult = checkLocalTime(startTime, lParams.longitude);
		tm* gmt;
		gmt = getGMTFromLocal(startTime, lParams.longitude);
		switch (checkResult)
		{
		case 1:
			lt.tm_min = gmt->tm_min;
			lt.tm_hour = gmt->tm_hour;
			lt.tm_day=gmt->tm_mday;
			lt.tm_mon = gmt->tm_mon;
			lt.tm_year = gmt->tm_year;
			lParams.radarCode = toLower(lParams.radarCode);
			monthDataNight[lt] = lParams;
			break;
		case 2:
			lt.tm_min = gmt->tm_min;
			lt.tm_hour = gmt->tm_hour;
			lt.tm_day = gmt->tm_mday;
			lt.tm_mon = gmt->tm_mon;
			lt.tm_year = gmt->tm_year;
			lParams.radarCode = toLower(lParams.radarCode);
			monthDataDay[lt] = lParams;
			break;
		case 0:
		default:
			lParams.radarCode += '?';
			monthData[lt] = lParams;
			break;
		}
		
	}
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
	is.set_delimiter(';', "$$");
	if (is.is_open())
	{
		is.read_line(); //HEADER
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
				try
				{
					is >> s_data;
					lp.strparams.push_back(s_data);
				}
				catch (...)
				{
					//break; //OLD VERSION?
				}
			}
			for (unsigned char i = 0; i != NUMPARAMETERS; i++)
			{
				try
				{
					is >> data;
					lp.params.push_back(data);
				}
				catch (...)
				{
					//break; //OLD VERSION?
				}
				
			}
			try
			{
				lt = parseTime(datetime);
			}
			catch (...){}

			/*
			
			if (lt.tm_hour > 10 && lt.tm_hour < 20)
				monthDataDay[lt] = lp;
			else
				monthDataNight[lt] = lp;

			*/

			if (lt.isDayLaunch())
				monthDataDay[lt] = lp;
			else if (lt.isNightLaunch())
				monthDataNight[lt] = lp;
			else
				monthData[lt] = lp;
		}
	}
	else
	{
		cout << "ERROR Opening CSV file." << endl;
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

void CSVWorker::setOperationType(int t)
{
	type = t;
}

void CSVWorker::writeCSV(string filename)
{
	csv::ofstream os(filename);
	os.set_delimiter(';', "$$");
	if (os.is_open())
	{
		os << "YYYY.MM.DD hh:mm" << "RADAR" << "FMTS" << "TIME" << "H" << "KN04CODE" << "D" << "MINEL" << "A10EL" << "Q" << "GT" << "MT" << "IZT" << "WD" << "WS" << "SPKS" <<"SHGHT"<<"LERRT"<<"LERRH"<< NEWLINE;
		
		
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
		it1 = monthData.begin();
		while (it1 != monthData.end())
		{
			writeLaunch(os, (*it1).first, (*it1).second);
			it1++;
		}
	}
	os.flush();
}
