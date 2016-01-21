// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdlib.h> 
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <math.h> 
#include <time.h>
#include "Pvector.h"


enum State {
	SEARCH,
	ATTACK,
	POWERUP,
	AVOIDASTEROID,
	RETREAT
};


using namespace sf;
using namespace std; 
