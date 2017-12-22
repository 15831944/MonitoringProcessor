#include "TAE3.h"
#include <algorithm> 
#include <cctype>
#include <locale>

TAE3::TAE3()
{
	sumcos = 0;
	sumsin = 0;
	avV = 0;
	avD = 0;
	dcnt = 0;
	sunh = 0;
	terr = 0;
	herr = 0;
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static inline string clr_str(std::string s) {
	
	string res = "";
	for (int i = 0; i != s.length(); i++)
	{
		if (((s[i] >= '0') && (s[i] <= '9')) || (s[i] == ' ') || (s[i] == '-') || (s[i] == '.') || (s[i] == '.'))
		{
			res += s[i];
		}
	}
	return res;
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		rtrim(token);
		ltrim(token);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void TAE3::addString(string str)
{
	string str1 = str;
	str1 = clr_str(str1);
	rtrim(str1);
	ltrim(str1);
	vector<string> data = split(str1," ");
	if (data.size() < 6)
		return;
	int V = atoi(data[5].c_str());
	int D = atoi(data[4].c_str());
	char d_digits[5];
	float H = atof(data[0].c_str());
	int Hi = (int)(H * 1000);
	if ((Hi >= 3000) && (Hi <= 10000))
	{
		if ((D > 0) && (V > 0))
		{
			//Так вычисляется средний угол. См википедию.
			sumcos += cos((float)D / 180 * 3.1415926535);
			sumsin += sin((float)D / 180 * 3.1415926535);
			avV += (float)V;
			dcnt++;
		}
	}
}

void TAE3::addString_legacy(string str2)
{
	//string str1 = "ИП      1.340   850.0  -12.8  26    8    8   15.6\r";
	try
	{
		string str1 = str2;
		string len = "ИП      1.340   850.0  -12.8  26    8";
		rtrim(str2);
		if (len.length() > str2.length())
			return;
		//Найдём столбы с D,V и H.
		char d_digits[5] = { str1[len.length() - 1], str1[len.length() - 2], str1[len.length() - 3], 0, 0 };
		for (int i = 0; i != 3; i++)
		{
			if (!isdigit(d_digits[i]))
			{
				d_digits[i] = 0;
			}
			else
			{
				d_digits[i] -= '0';
			}
		}
		int D = d_digits[2] * 100 + d_digits[1] * 10 + d_digits[0]; //D ok
		len = "ИП      1.340   850.0  -12.8  26    8    8";
		d_digits[2] = str1[len.length() - 3];
		d_digits[1] = str1[len.length() - 2];
		d_digits[0] = str1[len.length() - 1];
		for (int i = 0; i != 3; i++)
		{
			if (!isdigit(d_digits[i]))
			{
				d_digits[i] = 0;
			}
			else
			{
				d_digits[i] -= '0';
			}
		}
		int V = d_digits[2] * 100 + d_digits[1] * 10 + d_digits[0];

		len = "ИП      1.340";
		d_digits[4] = str1[len.length() - 6];
		d_digits[3] = str1[len.length() - 5];
		d_digits[2] = str1[len.length() - 3];
		d_digits[1] = str1[len.length() - 2];
		d_digits[0] = str1[len.length() - 1];
		for (int i = 0; i != 5; i++)
		{
			if (!isdigit(d_digits[i]))
			{
				d_digits[i] = 0;
			}
			else
			{
				d_digits[i] -= '0';
			}
		}
		int H = d_digits[4] * 10000 + d_digits[3] * 1000 + d_digits[2] * 100 + d_digits[1] * 10 + d_digits[0];
		if ((H >= 3000) && (H <= 10000))
		{
			if ((D > 0) && (V > 0))
			{
				//Так вычисляется средний угол. См википедию.
				sumcos += cos((float)D / 180 * 3.1415926535);
				sumsin += sin((float)D / 180 * 3.1415926535);
				avV += (float)V;
				dcnt++;
			}
		}
	}
	catch (...)
	{

	}
}

void TAE3::addSunString(string sun)
{
	string s_sun = clr_str(sun);
	sunh = atof(s_sun.c_str());
}

void TAE3::addTempErrorString(string terror)
{
	string s_terror = clr_str(terror);
	terr = atof(s_terror.c_str());
}

void TAE3::addHumErrorString(string humerror)
{
	string s_hum = clr_str(humerror);
	herr = atof(s_hum.c_str());
}

void TAE3::compute()
{
	//sumcos /= dcnt;
	//sumsin /= dcnt;
	if (dcnt)
	{
		avD = atan2(sumsin, sumcos)*180/3.1415926535;
		avV /= (float)dcnt;
	}
}