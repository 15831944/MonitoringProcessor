#include "TestBin.h"

void TestBin::addFiles(vector<string> names)
{
	mNames = names;
	for (auto i : names)
	{
		markedNames[i] = false;
	}
}

void TestBin::markFileAsRead(string filename)
{
	markedNames[filename] = true;
}

void TestBin::generateReport()
{
	csv::ofstream os("used_report.csv");
	os.set_delimiter(';', "$$");
	if (os.is_open())
	{

	}
}
