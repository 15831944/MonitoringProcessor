// MProc.cpp : Defines the entry point for the console application.
//

//arg1 = infile

#include "stdafx.h"
#include <sstream>      // std::ostringstream
#include "miniz.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	int station_index;

	if(argc <= 1)
    {
        std::cout << "usage: " << argv[0] << " filename" << std::endl;
        return 0;
    }

	//string infile = string(argv[1]);
	
	//std::istringstream oss(infile.substr(0,infile.find('-')));
	//oss>>station_index;

	
	return 0;
}

