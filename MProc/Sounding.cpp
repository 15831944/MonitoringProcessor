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

void Sounding::processRAWFile()
{
	string rawFile = getRAWfile();
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
		int newl = rawFile.find('\n');
		for (int i = 0; i != 9; i++)
		{
			newl = rawFile.find('\n',newl)+1;
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

