#ifndef RAWDATA_H
#define RAWDATA_H

#include "stdafx.h"

class RAWData
{
public:
	RAWData()
	{
		maxAltitude = 0;
	};
	~RAWData(){};
	void addString(string str)
	{
		int time;
		float mH, mD, mE, mA, mT, mU;
		sscanf_s(str.c_str(), "%d %f %f %f %f %f %f", &time, &mH, &mD, &mE, &mA, &mT, &mU);
		H[time] = mH;
		D[time] = mD;
		E[time] = mE;
		A[time] = mA;
		T[time] = mT;
		U[time] = mU;
		if (maxAltitude < mH)
			maxAltitude = mH;
	}
	int getSoundingTime()
	{
		if (H.size() == 0)
			return 0;
		map<int, float>::iterator it = H.end();
		it--;
		return (*it).first;
	}
	float getMaxAltitude()
	{
		return maxAltitude;
	}
	map<int,float> H, D, E, A, T, U;
private:
	float maxAltitude;
};

#endif