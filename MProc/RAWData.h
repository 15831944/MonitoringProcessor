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
		MarkGen& mg = MarkGen::Instance();
		string arr[7];
		//sscanf_s(str.c_str(), "%d %f %f %f %f %f %f", &time, &mH, &mD, &mE, &mA, &ssT, &ssU);
		stringstream ssin(str);
		int i=0;
		while (ssin.good() && i < 7){
			ssin >> arr[i];
			++i;
		}
		i = 0;
		try
		{
			time = atoi(arr[i].c_str());
		}
		catch (...)
		{
			time = 0;
		}
		i++;
		try
		{
			mH = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mH = 0;
		}
		i++;
		try
		{
			mD = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mD = 0;
		}
		i++;
		try
		{
			mE = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mE = 0;
		}
		i++;
		try
		{
			mA = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mA = 0;
		}
		i++;
		try
		{
			mT = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mT = 0;
		}
		i++;
		try
		{
			mU = (float)atof(arr[i].c_str());
		}
		catch (...)
		{
			mU = 0;
		}

		H[time] = mH;
		D[time] = mD;
		E[time] = mE;
		A[time] = mA;
		T[time] = mT;
		U[time] = mU;

		/**/

		

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

		if (mg.getMarkEnable(2))
		{
			DispersionCalculator* dc = mg.getDispersionCalculator(0);
			dc->dataInput(time, mH);
			dc = mg.getDispersionCalculator(1);
			dc->dataInput(time, mD);
			dc = mg.getDispersionCalculator(2);
			dc->dataInput(time, mE);
			dc = mg.getDispersionCalculator(3);
			dc->dataInput(time, mA);
			dc = mg.getDispersionCalculator(4);
			dc->dataInput(time, mT);
			dc = mg.getDispersionCalculator(5);
			dc->dataInput(time, mU);
		}
	}
	int getSoundingTime()
	{
		if (H.size() == 0)
			return 0;
		map<int, float>::iterator it = H.end();
		it--;
		return (*it).first;
	}
	float getGroundTemperature()
	{
		float t;
		if (T.size() < 100)
			return T[0];
		for (int i = 0; i != 60; i++)
		{
			t += T[i];
		}
		t /= 60;
		return t;
	}

	float getMinTemperature()
	{
		float t=T[0];
		for (int i = 1; i != T.size(); i++)
		{
			if (T[i] < t)
				t = T[i];
		}
		return t;
	}
	float getMinTemperature(float Hthr)
	{
		int cnt=0;
		for (int i = 1; i != T.size(); i++)
		{
			if (H[i] > Hthr)
				cnt = i;
		}
		float t = T[cnt];
		for (int i = cnt; i != T.size(); i++)
		{
			if (T[i] < t)
				t = T[i];
		}
		return t;
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
	//float groundTemp;
	float maxAltitude;
	float maxDistance;
	float minElevation;
	float alt10Elevation;
};

#endif