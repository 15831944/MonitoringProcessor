// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	int station_index,year,month,day;

	day = 5;

	if(argc <= 1)
    {
        std::cout << "usage: " << argv[0] << " filename" << std::endl;
        return 0;
    }

	string infile = string(argv[1]);
	
	std::istringstream oss(infile.substr(0,infile.find('-')));
	oss>>station_index;
	oss.clear();
	oss = std::istringstream(infile.substr(infile.find('-')+1,4));
	oss >> year;
	oss.clear();
	oss = std::istringstream(infile.substr(infile.find('-')+5, 2));
	oss >> month;
	Sounding s;
	string datam[N];
	string datan[N];
	string data;

	std::ostringstream str_d;

	str_d << day << '/' << day << '.' << month << '.' << year << "-11.30";

	string morningstr = str_d.str();
	string nightstr = "5/5.11.2016-23.31";

	try
	{
		zip_file file(argv[1]);
		file.printdir();

		for (i = 0; i != N;i++)
		{
			s.setDayOrNight(0);
			try
			{
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
		cout << "#1 Error reading file " << argv[1] << endl;
	}
	system("pause");
	return 0;
}

