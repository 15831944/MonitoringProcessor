#ifndef RAWDATA_H
#define RAWDATA_H

#include "stdafx.h"

class RAWData
{
public:
	RAWData()
	{
		maxAltitude = 0;
		maxDistance = 0;
		alt10Elevation = -1;
		minElevation = -1;
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
		if ((minElevation > 10.0f) && (mE <= 10.0f))
		{
			alt10Elevation = mH;
		}
		if (((minElevation < 0) || (minElevation > mE)) && (time>60))
			minElevation = mE;
		if (maxAltitude < mH)
			maxAltitude = mH;
		if (maxDistance < mD)
			maxDistance = mD;
	}
	int getSoundingTime()
	{
		if (H.size() == 0)
			return 0;
		map<int, float>::iterator it = H.end();
		it--;
		return (*it).first;
	}
	inline float getMaxAltitude()
	{
		return maxAltitude;
	}
	inline float getMaxDistance()
	{
		return maxDistance;
	}
	inline float getMinElevation()
	{
		return minElevation;
	}
	inline float getAlt10Elevation()
	{
		return alt10Elevation;
	}
	map<int,float> H, D, E, A, T, U;
private:
	float maxAltitude;
	float maxDistance;
	float minElevation;
	float alt10Elevation;
};

#endif