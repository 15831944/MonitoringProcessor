// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"

#include "zip_file.hpp"
#include "Sounding.h"
#include "CSVWorker.h"
#include "INIReader.h"
#include "MarkGenerator.h"
#include "RadarReader.h"

#include "MProcUtils.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int i=0;
	int mode=1;
	int radar = 1;
	RadarReader* mReader = new RadarReader();
	csvw = new CSVWorker();
	//TODO ADD pre-load CSV
	
	day = 1;
	stringstream ss;
	std::istringstream oss;
	size_t pos=-1;
	string datam[N];
	string datan[N];
	string data;

	if (!printUsageStrings(argc, argv)) // Вывод строки как использовать. 
		return 0;

	readSetupFile(); // Чтение INI файла настроек
	processInputParameters(argc, argv); // Создаёт infile и infile2 (варианты наименования)
	makeInOutFilenames(); // Создаёт infile и infile2 (варианты наименования)

	string morningstr;
	string nightstr;

	mReader->setSettings(settings);
	mReader->setCSVWorker(csvw);

	try
	{
		//TODO CHECK DATA ALREADY EXISTS!!! - OK
		readCSVFile();
		
		for (radar = 0; radar != NUMRADARS; radar++)
		{
			try
			{
				mReader->init(radar1[radar], radar2[radar], raw_radar[radar], radar_name[radar]);
				mReader->initFromFile(infile, infile2);
				mReader->setRadarNumber(radar);
				mReader->processMonthInd(month, year);
			}
			catch (...)
			{

			}
		}
		csvw->writeCSV(outfile);
		TestBin::Instance().generateReport("rep_"+outfile);
	}
	catch (...)
	{
		cout << "#4 Unknown error " << infile + radar2[radar] << endl;
	}
	if (final_prompt)
		system("pause");
	return 0;
}

