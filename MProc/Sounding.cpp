#include "Sounding.h"

void Sounding::addData(string data, string format)
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

bool Sounding::hasFormat(string format)
{
	if (!dayornight)
	{
		return mDataDay[format].length()>0;
	}
	else
	{
		return mDataNight[format].length()>0;
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
	if (!dayornight)
	{
		return mDataDay[format];
	}
	else
	{
		return mDataNight[format];
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
			if (!dayornight)
				infozond1 = zond_type;
			else
				infozond2 = zond_type;
		}
	}
	catch (...)
	{

	}
}

int Sounding::getInfoZondType()
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

void Sounding::processRAWFile()
{
	string rawFile = getRAWfile();
	try
	{
		if (rawFile.length() > 0)
		{
			/*
			ÈÑÕÎÄÍÛÅ ÄÀÍÍÛÅ ÇÎÍÄÈĞÎÂÀÍÈß ÇÀ 05.11.2016 11:30
			ÏĞÈÇÅÌÍÛÉ ÂÅÒÅĞ (D V) :  22  6
			ÏĞÈÇÅÌÍÎÅ ÄÀÂËÅÍÈÅ : 1011.0
			ĞÀÑÏÎËÎÆÅÍÈÅ ÑÒÀÍÖÈÈ :
			ØÈĞÎÒÀ :  43 ÄÎËÃÎÒÀ : 132
			ÂÛÑÎÒÀ ÍÀÄ ÓĞÎÂÍÅÌ ÌÎĞß :    90
			ÑÈÍÎÏÒÈ×ÅÑÊÈÉ ÈÍÄÅÊÑ ÑÒÀÍÖÈÈ : 31977
			ÊÎÄ ÎÁËÀ×ÍÎÑÒÈ :
			ÂĞÅÌ     H      D      E      A     T  U
			*/
			unsigned int newl = rawFile.find('\n');
			for (int i = 0; i != 9; i++)
			{
				newl = rawFile.find('\n', newl) + 1;
			}

			while (newl < rawFile.length())
			{
				string last_str = rawFile.substr(newl, rawFile.find('\n', newl) - newl);
				if (!dayornight)
					rDaM.addString(last_str);
				else
					rDaN.addString(last_str);
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
			
			ÒÀÁËÈÖÀ ĞÅÇÓËÜÒÀÒÎÂ ÇÎÍÄÈĞÎÂÀÍÈß ÒÀİ-3
			ÍÀ×ÀËÎ ÍÀÁËŞÄÅÍÈÉ : 01.03.2017 11:30
			ÊÎÍÅÖ ÍÀÁËŞÄÅÍÈÉ  : 01.03.2017 12:59
			ÂÛÑÎÒÀ ÑÎËÍÖÀ : 7 ÃĞÀÄ.
			ÑÈÍÎÏÒÈ×ÅÑÊÈÉ ÈÍÄÅÊÑ ÑÒÀÍÖÈÈ : 23933
			ÊÎÄ ÎÁËÀ×ÍÎÑÒÈ : 00900
			ÏĞÈÇÅÌÍÀß ÎØÈÁÊÀ ÒÅÌÏÅĞÀÒÓĞÛ : 0.3 ÃĞÀÄ.
			ÏĞÈÇÅÌÍÀß ÎØÈÁÊÀ ÂËÀÆÍÎÑÒÈ   : 6.8 %
		H       P       T    U   D    V   TD
			*/

			//Ïğîïóñòèòü 9 ñòğîê
			unsigned int newl = tae3File.find('D');
			newl = tae3File.find('\n', newl)+1;
			/*for (int i = 0; i != 9; i++)
			{
				newl = tae3File.find('\n', newl) + 1;
			}*/

			//êîíåö ñòğîêè
			while (newl < tae3File.length()-4)
			{
				string last_str = tae3File.substr(newl, tae3File.find('\n', newl) - newl);
				if (!dayornight)
					tDaM.addString(last_str);
				else
					tDaN.addString(last_str);
				newl = tae3File.find('\n', newl) + 1;
			}
			if (!dayornight)
				tDaM.compute();
			else
				tDaN.compute();


		}
	}
	catch (...)
	{
		cout << "Error reading TAE3!" << endl;
	}

}

int Sounding::getKN04Code()
{
	if (!dayornight)
		return KN04Code1;
	return KN04Code2;
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
			if (!dayornight)
				is >> KN04Code1;
			else
				is >> KN04Code2;
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
		if (!dayornight)
		{
			return tDaM.getAverageWindDirection();
		}
		else
		{
			return tDaN.getAverageWindDirection();
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
		if (!dayornight)
		{
			return tDaM.getAverageWindSpeed();
		}
		else
		{
			return tDaN.getAverageWindSpeed();
		}
	}
	catch (...)
	{
	}
	return 0;
}

int Sounding::getRAWSoundingTime()
{
	if (!dayornight)
	{
		return (int)rDaM.getSoundingTime();
	}
	else
	{
		return (int)rDaN.getSoundingTime();
	}
	return 0;
}

int Sounding::getMaxAltitude()
{
	if (!dayornight)
	{
		return (int)rDaM.getMaxAltitude();
	}
	else
	{
		return (int)rDaN.getMaxAltitude();
	}
	return 0;
}

int Sounding::getMaxDistance()
{
	if (!dayornight)
	{
		return (int)rDaM.getMaxDistance();
	}
	else
	{
		return (int)rDaN.getMaxDistance();
	}
	return 0;
}

float Sounding::getMinElevation()
{
	if (!dayornight)
	{
		return rDaM.getMinElevation();
	}
	else
	{
		return rDaN.getMinElevation();
	}
	return 0;
}

float Sounding::getGroundTemperature()
{
	if (!dayornight)
	{
		return rDaM.getGroundTemperature();
	}
	else
	{
		return rDaN.getGroundTemperature();
	}
	return 0;
}

float Sounding::getMinTemperature()
{
	if (!dayornight)
	{
		return rDaM.getMinTemperature();
	}
	else
	{
		return rDaN.getMinTemperature();
	}
	return 0;
}

float Sounding::getMinTemperature(float H)
{
	if (!dayornight)
	{
		return rDaM.getMinTemperature(H);
	}
	else
	{
		return rDaN.getMinTemperature(H);
	}
	return 0;
}

int Sounding::getAlt10Elevation()
{
	if (!dayornight)
	{
		return (int)rDaM.getAlt10Elevation();
	}
	else
	{
		return (int)rDaN.getAlt10Elevation();
	}
	return 0;
}

