#include "RadarReader.h"

RadarReader::RadarReader()
{

}

RadarReader::~RadarReader()
{

}

void RadarReader::init(string radarPrefix, string radarZIP, string rawPrefix, string radarName)
{
	mRadarPrefix = radarPrefix;
	mRadarZip = radarZIP;
	mRawPrefix = rawPrefix;
	mRadarName = radarName;
}

void RadarReader::initFromFile(string infile, string infile2)
{
	string test_pr = infile;
	if (!fileExists(infile + mRadarZip))
		test_pr = infile2;
	zip_file file(test_pr + mRadarZip);
	base = file.get_first_filename();
	if (base.find_last_of('/') != string::npos)
		base = base.substr(0, base.find_last_of('/')) + "/";
	else
		base = "";
}

void RadarReader::processMonth()
{
	for (day = 1; day <= daysInMonth(month); day++)
	{
		Sounding s;
		s.setRAWDataIdentifier(mRawPrefix);
		ss.clear();
		ss = stringstream();

		if (radar != 2)
		{
			ss << base << day << '.' << month << '.' << year;// << "-11.30";
		}
		else
		{
			char date[10];
			sprintf_s(date, "%04d%02d%02d", year, month, day);
			ss << base << string(date);
		}

		morningstr = ss.str();
		nightstr = morningstr;

		cout << morningstr << endl;
		for (i = 0; i != N; i++)
		{
			s.setDayOrNight(0);
			try
			{
				data = readFormat(file, morningstr, 0, formats[i]);//file.read(morningstr + formats[i]);
				datam[i] = data;
				s.addData(data, formats[i]);
			}
			catch (...)
			{
				cout << "#2 Error reading format " << formats[i] << endl;
				datam[i] = "";
				s.addData("", formats[i]);
			}
			s.setDayOrNight(1);
			try
			{
				data = readFormat(file, nightstr, 1, formats[i]);
				datan[i] = data;
				s.addData(data, formats[i]);
			}
			catch (...)
			{
				cout << "#3 Error reading format " << formats[i] << endl;
				datan[i] = "";
				s.addData("", formats[i]);
			}
		}

		LaunchTime lt1;
		lt1.tm_hour = 11;
		lt1.tm_min = 30;
		LaunchTime lt2;
		lt2.tm_hour = 23;
		lt2.tm_min = 30;

		for (int cnt = 0; cnt <= 1; cnt++)
		{
			s.setDayOrNight(cnt);
			s.processRAWFile();
			s.processINFOFile();
			if (mSettings[NUMPARAMETERS_STR + 10])
				s.processTAE3File();
			if (mSettings[NUMPARAMETERS_STR + 2])
				s.processKN04File();
			//yyyy.mm.dd hh:mm
			LaunchTime lt;
			lt1.tm_year = year;
			lt1.tm_mon = month;
			lt1.tm_day = day;
			lt2.tm_year = year;
			lt2.tm_mon = month;
			lt2.tm_day = day;
			if (cnt == 0)
			{
				lt = lt1;
			}
			else
			{
				lt = lt2;
			}

			MarkGen& mg = MarkGen::Instance();

			LaunchParameters l;
			l.radarCode = radar1[radar];

			int t1 = s.getRAWSoundingTime();
			int t2 = s.getSoundingTime();

			mg.setSoundingTime(t2);

			push_back_params(l, s, mSettings, error_str);

			if (t1 || t2)
				csvw.addLaunch(cnt, lt, l);
			printSoundingInformation(cnt, s);
			mg.clearCalcs();
		}
	}
}