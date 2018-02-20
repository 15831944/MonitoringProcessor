#ifndef TESTBIN_H
#define TESTBIN_H

#include "stdafx.h"

using namespace mini;

class TestBin
{
public:
	static TestBin& Instance()
	{
		// Singleton
		static TestBin s;
		return s;
	}
	void addFiles(vector<string> names);
	void markFileAsRead(string filename);
	void generateReport();
private:
	TestBin()
	{
		
	};
	~TestBin()
	{
	};

	TestBin(TestBin const&); // реализация не нужна
	TestBin& operator= (TestBin const&);  // и тут

	vector<string> mNames;
	map<string, bool> markedNames;
};

#endif