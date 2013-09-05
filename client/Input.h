#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include <iostream>

struct Mouse
{
	static Mouse* getInstance();

private:
	Mouse();

	static bool instanceFlag;
	static Mouse *single;
};

#endif
