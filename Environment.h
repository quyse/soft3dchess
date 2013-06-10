#ifndef ___ENVIRONMENT_H___
#define ___ENVIRONMENT_H___

#include "math.h"
#include <utility>

struct Environment
{
	vector<3> lightPosition;
	std::pair<int, int> selectedCell;
};

#endif
