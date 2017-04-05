#include "TAE3.h"

TAE3::TAE3()
{
	sumcos = 0;
	sumsin = 0;
	avV = 0;
	dcnt = 0;
}


void TAE3::addString(string str2)
{
	//string str1 = "ИП      1.340   850.0  -12.8  26    8    8   15.6\r";
	string str1 = str2;
	string len = "ИП      1.340   850.0  -12.8  26    8";
	if (len.length() > str2.length())
		return;
	//Найдём столбы с D,V и H.
	char d_digits[5] = { str1[len.length()-1], str1[len.length()-2], str1[len.length()-3],0,0 };
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