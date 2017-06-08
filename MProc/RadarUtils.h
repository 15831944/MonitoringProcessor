#include "stdafx.h"

#ifndef RADARUTILS_H
#define RADARUTILS_H


inline bool fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

#endif