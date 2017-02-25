#ifndef MARKGEN_H
#define MARKGEN_H

#include "stdafx.h"

#define MARKS_NUMBER 2

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
private:
	MarkGen()
	{
		for (int i = 0; i != MARKS_NUMBER; i++)
		{
			mark_weights[i] = 1;
		}
	};
	~MarkGen()
	{
	};

	MarkGen(MarkGen const&); // реализация не нужна
	MarkGen& operator= (MarkGen const&);  // и тут

	int marks[MARKS_NUMBER];
	int mark_weights[MARKS_NUMBER];
};

#endif