#include "DayFormats.h"

void DayFormats::addData(string data, string format)
{
	mData[format] = data;
}

bool DayFormats::hasFormat(string format)
{
	return mData[format].length()>0;
}

void DayFormats::setLaunchTime(LaunchTime lt)
{
	mLt = lt;
}

int DayFormats::checkFormats()
{
	int c = 0;
	for (auto i : formats)
	{
		if (hasFormat(i))
			c++;
	}
	return c;
}

string DayFormats::getFormatsTelegram()
{
	int it2 = 0;
	string res = "";
	for (auto i : formats)
	{
		if (hasFormat(i))
		{
			res += formats_sh[it2];
		}
		it2++;
	}
	return res;
}

string DayFormats::getFormat(string format)
{
	return mData[format];
}