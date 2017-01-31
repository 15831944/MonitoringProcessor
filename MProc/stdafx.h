// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <sstream> 
#include <map>
#include <iostream>
#include <vector>

#include "minicsv.h"
#include "LaunchParameters.h"

#define N 14

using namespace std;

#define NUMRADARS 3

static std::string formats[N] = { ".AB..GROUND", ".crd", ".GROUND", ".info", ".KN04", ".KN4", ".LAYERS", ".prof", ".prof.1", ".RAW", ".SP", ".TAE03", ".TAE3", ".tu" };
static std::string radar1[NUMRADARS] = { "A", "R", "V" };
static std::string radar2[NUMRADARS] = { "A.zip", "R.zip", "V.zip" };
// TODO: reference additional headers your program requires here
