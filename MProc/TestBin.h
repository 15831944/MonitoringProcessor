#define TESTBIN_H

#include "stdafx.h"
#include "DispersionCalc.h"
#include "DerivativeCalc.h"

#define MARKS_NUMBER 4
#define DISPERSION_CALCULATORS 6

#define CALCULATORS 12

class TestBin
{
public:
	static TestBin& Instance()
	{
		// Singleton
		static TestBin s;
		return s;
	}
private:
	TestBin()
	{
		
	};
	~TestBin()
	{
	};

	TestBin(TestBin const&); // реализация не нужна
	TestBin& operator= (TestBin const&);  // и тут
};

#endif