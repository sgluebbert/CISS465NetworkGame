#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include <iostream>

struct Mouse
{
	static Mouse* getInstance();
	bool leftPressed, leftDown, leftReleased;
	bool rightPressed, rightDown, rightReleased;

	void reset();
	void handlePress(Uint8);
	void handleRelease(Uint8);

private:
	Mouse();

	static bool instanceFlag;
	static Mouse *single;
};

#endif
