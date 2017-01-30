// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"

using namespace std;
int station_index, year, month, day;
string s_station_index, s_year, s_month;



int strToInt(string myString)
{
	istringstream buffer(myString);
	int value;
	buffer >> value;
	return value;
}

void recognizeToken(string arg)
{
	if (arg.find('Y') != string::npos)
	{
		s_year = arg.substr(arg.find('Y')+2,arg.length()-2);
		year = strToInt(s_year);
	}
	if (arg.find('M') != string::npos)
	{
		s_month = arg.substr(arg.find('M') + 2, arg.length() - 2);
		month = strToInt(s_month);
	}
	if (arg.find('I') != string::npos)
	{
		s_station_index = arg.substr(arg.find('I') + 2, arg.length() - 2);
		month = strToInt(s_station_index);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	int mode;
	int radar = 1;
	day = 9;

	if(argc <= 1)
    {
        std::cout << "usage: " << argv[0] << " filename" << std::endl;
		std::cout << "usage: " << argv[0] << " yyyy iiiii mm" << std::endl;
        return 0;
    }

	string infile;
	stringstream ss;
	std::istringstream oss;
	size_t pos;



	switch (argc)
	{
	case 2:
		infile = string(argv[1]);
		pos = infile.find_last_of('/');
		if (pos == std::string::npos)
			pos = 0;
		oss = std::istringstream(infile.substr(pos, infile.find('-')));
		oss >> station_index;
		oss.clear();
		oss = std::istringstream(infile.substr(infile.find('-') + 1, 4));
		oss >> year;
		oss.clear();
		oss = std::istringstream(infile.substr(infile.find('-') + 5, 2));
		oss >> month;
		break;
	case 4:
		/*ss << argv[1] << '/' << argv[2] << '/' << argv[2] << '-' << argv[1] << argv[3] << "A.zip";
		infile = ss.str();
		oss = std::istringstream(argv[1]);
		oss >> year;
		oss.clear();
		oss = std::istringstream(argv[3]);
		oss >> month;
		oss.clear();
		oss = std::istringstream(argv[2]);
		oss >> station_index;
		oss.clear();*/

		for (i = 1; i != 4; i++)
			recognizeToken(string(argv[i]));

		ss << s_year << '/' << s_station_index << '/' << s_station_index << '-' << s_year << s_month << radar1[radar] << ".zip";
		infile = ss.str();
		oss = std::istringstream(argv[1]);
		oss >> year;
		oss.clear();
		oss = std::istringstream(argv[3]);
		oss >> month;
		oss.clear();
		oss = std::istringstream(argv[2]);
		oss >> station_index;
		oss.clear();
		break;
	default:
		break;
	}

	
	Sounding s;
	string datam[N];
	string datan[N];
	string data;

	ss.clear();
	ss = stringstream();
	//str_d << day << '/' << day << '.' << month << '.' << year << "-11.30";
	ss << s_station_index << '-' << s_year << s_month << radar1[radar] << '/' << month;
	ss << radar2[radar] << '/' << s_month << '/' << day << '.' << month << '.' << year;//<< "-11.30";
	string morningstr = ss.str() + "-11.30";
	string nightstr = ss.str() + "-23.35";

	try
	{
		zip_file file(infile);
		//file.printdir();
		//file.read("25703-201609R/9Œ/9.9.2016-11.30.RAW");
		for (i = 0; i != N;i++)
		{
			s.setDayOrNight(0);
			try
			{
				file.read("25703-201609R/9.9.2016-11.30.RAW");
				data = file.read(morningstr + formats[i]);
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
				data = file.read(nightstr + formats[i]);
				datan[i] = data;
				s.addData(data, formats[i]);
			}
			catch (...)
			{
				cout << "#3 Error reading format " << formats[i] <<endl;
				datan[i] = "";
				s.addData("", formats[i]);
			}
		}
		s.setDayOrNight(0);
		s.processRAWFile();
		cout << "-----------------------------------\n SUMMARY INFORMATION MORNING" << endl;
		cout << "Formats " << s.checkFormats() << " of 14" << endl;
		cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
		cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
		cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
		s.setDayOrNight(1);
		s.processRAWFile();
		cout << "-----------------------------------\n SUMMARY INFORMATION NIGHT" << endl;
		cout << "Formats " << s.checkFormats() << " of 14" << endl;
		cout << "Sounding time(CRD) " << s.getSoundingTime() << endl;
		cout << "Sounding time(RAW) " << s.getRAWSoundingTime() << endl;
		cout << "Max altitude(RAW) " << s.getMaxAltitude() << endl;
	}
	catch (...)
	{
		cout << "#1 Error reading file " << infile << endl;
	}
	system("pause");
	return 0;
}

