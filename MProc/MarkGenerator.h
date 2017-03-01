#ifndef MARKGEN_H
#define MARKGEN_H

#include "stdafx.h"
#include "DispersionCalc.h"

#define MARKS_NUMBER 2
#define DISPERSION_CALCULATORS 6
class MarkGen
{
public:
	static MarkGen& Instance()
	{
		// Singleton
		static MarkGen s;
		return s;
	}

	float getQualityMark();
	void setMaxAltitude(int alt)
	{
		if (alt > 20000)
			marks[0] = 10;
		if ((alt < 20000) && (alt>3000))
			marks[0] = (int)((float)(alt - 3000) / 1700);
		if (alt<3000)
			marks[0] = 0;
	}
	void setSoundingTime(int time)
	{
		if (time > 2000)
			marks[1] = 10;
		if ((time < 2000) && (time>1000))
			marks[1] = (int)((float)(time - 1000) / 1000);
		if (time<1000)
			marks[1] = 0;
	}

	inline void setMarkEnable(int num, bool en)
	{
		mark_enable[num] = en;
	}

	DispersionCalculator* getDispersionCalculator(int num)
	{
		return dispersion_calculators[num];
	}

	void clearDispersionCalcs()
	{
		for (int i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			dispersion_calculators[i]->clear();
		}
	}
private:
	MarkGen()
	{
		for (int i = 0; i != MARKS_NUMBER; i++)
		{
			mark_weights[i] = 1;
			mark_enable[i] = true;
		}

		for (int i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			DispersionCalculator* dc = new DispersionCalculator();
			dispersion_calculators[i] = dc;
		}
	};
	~MarkGen()
	{
	};

	MarkGen(MarkGen const&); // реализация не нужна
	MarkGen& operator= (MarkGen const&);  // и тут

	int marks[MARKS_NUMBER];
	int mark_weights[MARKS_NUMBER];
	bool mark_enable[MARKS_NUMBER];
	DispersionCalculator* dispersion_calculators[DISPERSION_CALCULATORS];
};

#endif