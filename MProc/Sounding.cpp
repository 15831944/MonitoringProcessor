#include "Sounding.h"

void Sounding::addData(string data, string format)
{
	if (!type)
	{
		if (!dayornight)
		{
			mDataDay[format] = data;
		}
		else
		{
			mDataNight[format] = data;
		}
	}
	else
	{
		if (!mAllData[curLaunchTime])
		{
			DayFormats* df = new DayFormats;
			df->addData(data, format);
			mAllData[curLaunchTime] = df;
		}
		else
		{
			mAllData[curLaunchTime]->addData(data, format);
		}
	}
	
}

int Sounding::getLongitude()
{
	return longitude;
}

bool Sounding::hasFormat(string format)
{
	if (!type)
	{
		if (!dayornight)
		{
			return mDataDay[format].length() > 0;
		}
		else
		{
			return mDataNight[format].length() > 0;
		}
	}
	else
	{
		return mAllData[curLaunchTime]->hasFormat(format);
	}
	return 0;
}

int Sounding::checkFormats()
{
	int c = 0;
	for (auto i : formats)
	{
		if (hasFormat(i))
			c++;
	}
	return c;
}

string Sounding::getFormatsTelegram()
{
	int it2 = 0;
	string res = "";
	for (auto i : formats)
	{
		if (hasFormat(i))
		{
			res += formats_sh[it2];
		}
		it2++;
	}
	return res;
}

string Sounding::getFormat(string format)
{
	if (!type)
	{
		if (!dayornight)
		{
			return mDataDay[format];
		}
		else
		{
			return mDataNight[format];
		}
	}
	else
	{
		return mAllData[curLaunchTime]->getFormat(format);
	}
	return "";
}

void Sounding::setDayOrNight(char don)
{
	dayornight=don;
}

void Sounding::setRAWDataIdentifier(string rdata)
{
	mRawData = rdata;
}

string Sounding::getCRDfile()
{
	if (hasFormat(".crd"))
	{
		return getFormat(".crd");
	}
	return "";
}

string Sounding::getRAWfile()
{
	if (hasFormat(".RAW"))
	{
		return getFormat(".RAW");
	}
	if (hasFormat(mRawData))
	{
		return getFormat(mRawData);
	}
	return "";
}

string Sounding::getKN04file()
{
	if (hasFormat(".KN04"))
	{
		return getFormat(".KN04");
	}
	if (hasFormat(".KN4"))
	{
		return getFormat(".KN4");
	}
	return "";
}

void Sounding::setOperationType(int t)
{
	type = t;
}

void Sounding::setNewTime(LaunchTime newlTime)
{
	curLaunchTime = newlTime.launchTime;
}

string Sounding::getINFOfile()
{
	if (hasFormat(".info"))
	{
		return getFormat(".info");
	}
	return "";
}
string Sounding::getTAE3file()
{
	if (hasFormat(".TAE03"))
	{
		return getFormat(".TAE03");
	}
	if (hasFormat(".TAE3"))
	{
		return getFormat(".TAE3");
	}
	return "";
}

int Sounding::getSoundingTime()
{
	string data = getCRDfile();
	data[data.length()] = ' ';
	string last_str = data.substr(0, data.find_last_of('\t'));
	last_str = data.substr(last_str.find_last_of('\n')+1, data.length());
	float a, b, c, d;
	a = b = c = d = 0;
	//last_str = "1 2 3 4";
	sscanf_s(last_str.c_str(), "%f %f %f %f",&a,&b,&c,&d);
	return (int)a;
}

void Sounding::processINFOFile()
{
	string infoFile = getINFOfile();
	try
	{
		if (infoFile.length() > 0)
		{
			size_t newl = infoFile.find("RadioZondType");
			if (newl == string::npos)
				return;
			newl += 15;
			string last_str = infoFile.substr(newl, infoFile.find('\n', newl) - newl);
			int zond_type = 0;
			sscanf_s(last_str.c_str(), "%d", &zond_type);
			if (!type)
			{
				if (!dayornight)
					infozond1 = zond_type;
				else
					infozond2 = zond_type;
			}
			else
			{
				mAllData[curLaunchTime]->infozond = zond_type;
			}
			//мЮИД╦Л ДНКЦНРС ЯРЮМЖХХ(МСФМН ДКЪ ДЕРЕЙРХПНБЮМХЪ ЛЕЯРМНЦН БПЕЛЕМХ)
			newl = infoFile.find("StationLongitude");
			if (newl == string::npos)
				return;
			newl += string("StationLongitude").length()+2;
			last_str = infoFile.substr(newl, infoFile.find('\n', newl) - newl);
			
			sscanf_s(last_str.c_str(), "%d", &longitude);
		}
	}
	catch (...)
	{

	}
}

int Sounding::getInfoZondType()
{
	if (!type)
	{
		if (!dayornight)
		{
			return infozond1;
		}
		else
		{
			return infozond2;
		}
	}
	else
	{
		return mAllData[curLaunchTime]->infozond;
	}
}

void Sounding::processRAWFile()
{
	string rawFile = getRAWfile();
	try
	{
		if (rawFile.length() > 0)
		{
			/*
			хяундмше дюммше гнмдхпнбюмхъ гю 05.11.2016 11:30
			опхгелмши береп (D V) :  22  6
			опхгелмне дюбкемхе : 1011.0
			пюяонкнфемхе ярюмжхх :
			ьхпнрю :  43 днкцнрю : 132
			бшянрю мюд спнбмел лнпъ :    90
			яхмнорхвеяйхи хмдейя ярюмжхх : 31977
			йнд накювмнярх :
			бпел     H      D      E      A     T  U
			*/
			unsigned int newl = rawFile.find('\n');
			for (int i = 0; i != 9; i++)
			{
				newl = rawFile.find('\n', newl) + 1;
			}

			while (newl < rawFile.length())
			{
				string last_str = rawFile.substr(newl, rawFile.find('\n', newl) - newl);
				if (!type)
				{
					if (!dayornight)
						rDaM.addString(last_str);
					else
						rDaN.addString(last_str);
				}
				else
				{
					mAllData[curLaunchTime]->rDa.addString(last_str);
				}
				
				newl = rawFile.find('\n', newl) + 1;
			}

		}
	}
	catch (...)
	{

	}
	
}

void Sounding::processTAE3File()
{
	string tae3File = getTAE3file();
	try
	{
		if (tae3File.length() > 0)
		{
			/*
			
			рюакхжю пегскэрюрнб гнмдхпнбюмхъ рющ-3
			мювюкн мюакчдемхи : 01.03.2017 11:30
			йнмеж мюакчдемхи  : 01.03.2017 12:59
			бшянрю янкмжю : 7 цпюд.
			яхмнорхвеяйхи хмдейя ярюмжхх : 23933
			йнд накювмнярх : 00900
			опхгелмюъ ньхайю релоепюрспш : 0.3 цпюд.
			опхгелмюъ ньхайю бкюфмнярх   : 6.8 %
		H       P       T    U   D    V   TD
			*/

			//оПНОСЯРХРЭ 9 ЯРПНЙ
			unsigned int hsun = tae3File.find("янкмжю");
			unsigned int hum = tae3File.find("бкюфмнярх");
			unsigned int tmpr = tae3File.find("релоепюрспш");
			//бШДЕКХЛ ЯРПНЙС Я "БШЯНРНИ ЯНКМЖЮ" Х РД. ЯНУПЮМХЛ.
			string sunstring="";
			sunstring = tae3File.substr(hsun,tae3File.find('\n', hsun)-hsun);
			hsun = sunstring.find("цпюд");
			sunstring = sunstring.substr(0, hsun);
			mAllData[curLaunchTime]->tDa.addSunString(sunstring);
			
			string tempstring = "";
			tempstring = tae3File.substr(tmpr, tae3File.find('\n', tmpr) - tmpr);
			tmpr = tempstring.find("цпюд");
			tempstring = tempstring.substr(0, tmpr);
			mAllData[curLaunchTime]->tDa.addTempErrorString(tempstring);

			string humstring = "";
			humstring = tae3File.substr(hum, tae3File.find('\n', hum) - hum);
			hum = humstring.find("%");
			humstring = humstring.substr(0, hum);
			mAllData[curLaunchTime]->tDa.addHumErrorString(humstring);

			unsigned int newl = tae3File.find('D');
			newl = tae3File.find('\n', newl)+1;
			/*for (int i = 0; i != 9; i++)
			{
				newl = tae3File.find('\n', newl) + 1;
			}*/

			//ЙНМЕЖ ЯРПНЙХ
			while (newl < tae3File.length()-4)
			{
				string last_str = tae3File.substr(newl, tae3File.find('\n', newl) - newl);
				if (!type)
				{
					if (!dayornight)
						tDaM.addString(last_str);
					else
						tDaN.addString(last_str);
				}
				else
				{
					mAllData[curLaunchTime]->tDa.addString(last_str);
				}
				newl = tae3File.find('\n', newl) + 1;
			}

			if (!type)
			{
				if (!dayornight)
					tDaM.compute();
				else
					tDaN.compute();
			}
			else
			{
				mAllData[curLaunchTime]->tDa.compute();
			}


		}
	}
	catch (...)
	{
		cout << "Error reading TAE3!" << endl;
	}

}

float Sounding::getLandErrorHum()
{
	if (!type)
	{
		return 0;
	}
	else
	{
		return mAllData[curLaunchTime]->tDa.getHumError();
	}
}

float Sounding::getLandErrorTemp()
{
	if (!type)
	{
		return 0;
	}
	else
	{
		return mAllData[curLaunchTime]->tDa.getTempError();
	}
}

float Sounding::getSunHeight()
{
	if (!type)
	{
		return 0;
	}
	else
	{
		return mAllData[curLaunchTime]->tDa.getSunHeight();
	}
}

int Sounding::getKN04Code()
{
	if (!type)
	{
		if (!dayornight)
			return KN04Code1;
		return KN04Code2;
	}
	else
	{
		return mAllData[curLaunchTime]->KN04Code;
	}
}

void Sounding::processKN04File()
{
	string rawFile = getKN04file();
	try
	{
		if (rawFile.length() > 0)
		{
			
			unsigned int newl = rawFile.find("31313 ");
			
			string code = rawFile.substr(newl+6,5);
			istringstream is(code);
			if (!type)
			{
				if (!dayornight)
					is >> KN04Code1;
				else
					is >> KN04Code2;
			}
			else
			{
				is >> mAllData[curLaunchTime]->KN04Code;
			}
		}
	}
	catch (...)
	{

	}

}

float Sounding::getAverageWindDirection()
{
	try
	{
		if (!type)
		{
			if (!dayornight)
			{
				return tDaM.getAverageWindDirection();
			}
			else
			{
				return tDaN.getAverageWindDirection();
			}
		}
		else
		{
			return mAllData[curLaunchTime]->tDa.getAverageWindDirection();
		}
	}
	catch (...)
	{
	}
	return 0;
}

float Sounding::getAverageWindSpeed()
{
	try
	{
		if (!type)
		{
			if (!dayornight)
			{
				return tDaM.getAverageWindSpeed();
			}
			else
			{
				return tDaN.getAverageWindSpeed();
			}
		}
		else
		{
			return mAllData[curLaunchTime]->tDa.getAverageWindSpeed();
		}
	}
	catch (...)
	{
	}
	return 0;
}

int Sounding::getRAWSoundingTime()
{
	if (!type)
	{
		if (!dayornight)
		{
			return (int)rDaM.getSoundingTime();
		}
		else
		{
			return (int)rDaN.getSoundingTime();
		}
	}
	else
	{
		return (int)(mAllData[curLaunchTime]->rDa.getSoundingTime());
	}
	return 0;
}

int Sounding::getMaxAltitude()
{
	if (!type)
	{
		if (!dayornight)
		{
			return (int)rDaM.getMaxAltitude();
		}
		else
		{
			return (int)rDaN.getMaxAltitude();
		}
	}
	else
	{
		return (int)(mAllData[curLaunchTime]->rDa.getMaxAltitude());
	}
	return 0;
}

int Sounding::getMaxDistance()
{
	if (!type)
	{
		if (!dayornight)
		{
			return (int)rDaM.getMaxDistance();
		}
		else
		{
			return (int)rDaN.getMaxDistance();
		}
	}
	else
	{
		return (int)(mAllData[curLaunchTime]->rDa.getMaxDistance());
	}
	return 0;
}

float Sounding::getMinElevation()
{
	if (!type)
	{
		if (!dayornight)
		{
			return rDaM.getMinElevation();
		}
		else
		{
			return rDaN.getMinElevation();
		}
	}
	else
	{
		return (mAllData[curLaunchTime]->rDa.getMinElevation());
	}
	return 0;
}

float Sounding::getGroundTemperature()
{
	if (!type)
	{
		if (!dayornight)
		{
			return rDaM.getGroundTemperature();
		}
		else
		{
			return rDaN.getGroundTemperature();
		}
	}
	else
	{
		return mAllData[curLaunchTime]->rDa.getGroundTemperature();
	}
	return 0;
}

float Sounding::getMinTemperature()
{
	if (!type)
	{
		if (!dayornight)
		{
			return rDaM.getMinTemperature();
		}
		else
		{
			return rDaN.getMinTemperature();
		}
	}
	else
	{
		return mAllData[curLaunchTime]->rDa.getMinTemperature();
	}
	return 0;
}

float Sounding::getMinTemperature(float H)
{
	if (!type)
	{
		if (!dayornight)
		{
			return rDaM.getMinTemperature(H);
		}
		else
		{
			return rDaN.getMinTemperature(H);
		}
	}
	else
	{
		return mAllData[curLaunchTime]->rDa.getMinTemperature();
	}
	return 0;
}

int Sounding::getAlt10Elevation()
{
	if (!type)
	{
		if (!dayornight)
		{
			return (int)rDaM.getAlt10Elevation();
		}
		else
		{
			return (int)rDaN.getAlt10Elevation();
		}
	}
	else
	{
		return (int)(mAllData[curLaunchTime]->rDa.getAlt10Elevation());
	}
	return 0;
}

int Sounding::getNumSpikes()
{
	if (!type)
	{
		if (!dayornight)
		{
			return (int)rDaM.getNumSpikes();
		}
		else
		{
			return (int)rDaN.getNumSpikes();
		}
	}
	else
	{
		return (int)(mAllData[curLaunchTime]->rDa.getNumSpikes());
	}
	return 0;
}


