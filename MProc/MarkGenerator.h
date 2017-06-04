#ifndef MARKGEN_H
#define MARKGEN_H

#include "stdafx.h"
#include "DispersionCalc.h"
#include "DerivativeCalc.h"

#define MARKS_NUMBER 4
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
		{
			marks[0] = (int)((float)(alt - 3000) / 1700);
		}
			
		if (alt<3000)
			marks[0] = 0;
	}
	void setSoundingTime(int time)
	{
		if (time > 2000)
			marks[1] = 10;
		if ((time < 2000) && (time>1000))
			marks[1] = (int)((float)(time - 1000) / 100);
		if (time<1000)
			marks[1] = 0;
	}

	inline void setMarkEnable(int num, bool en)
	{
		mark_enable[num] = en;
	}

	inline bool getMarkEnable(int num)
	{
		return mark_enable[num];
	}

	DispersionCalculator* getDispersionCalculator(int num)
	{
		return (DispersionCalculator*)dispersion_calculators[num];
	}

	DerivativeCalculator* getDerivativeCalculator(int num)
	{
		return (DerivativeCalculator*)derivative_calculators[num];
	}

	void clearCalcs()
	{
		for (int i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			dispersion_calculators[i]->clear();
			derivative_calculators[i]->clear();
		}
	}
	inline void setDispersionCalcThreshold(int num, float thr)
	{
		dispersion_calculators[num]->setThresholdValue(thr);
	}
	inline void setDerivativeCalcThreshold(int num, float thr)
	{
		dispersion_calculators[num]->setThresholdValue(thr);
	}
	
private:
	MarkGen()
	{
		for (int i = 0; i != MARKS_NUMBER; i++)
		{
			marks[i] = 0;
			mark_weights[i] = 1;
			mark_enable[i] = true;
		}

		for (int i = 0; i != DISPERSION_CALCULATORS; i++)
		{
			DispersionCalculator* dc = new DispersionCalculator();
			dispersion_calculators[i] = dc;
			DerivativeCalculator* dc1 = new DerivativeCalculator();
			derivative_calculators[i] = dc1;
		}
	};
	~MarkGen()
	{
	};

	MarkGen(MarkGen const&); // реализация не нужна
	MarkGen& operator= (MarkGen const&);  // и тут

	void calcDispMark();

	int marks[MARKS_NUMBER];
	int mark_weights[MARKS_NUMBER];
	bool mark_enable[MARKS_NUMBER];
	Calculator* dispersion_calculators[DISPERSION_CALCULATORS];
	Calculator* derivative_calculators[DISPERSION_CALCULATORS];
};

#endif