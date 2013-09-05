#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include <iostream>
#include <bitset>

const int AMOUNT_OF_KEYS = 320;

struct Mouse
{
	static Mouse* getInstance();

	bool leftPressed, leftDown, leftReleased;
	bool rightPressed, rightDown, rightReleased;
	int x, y, dx, dy;

	void reset();
	void handlePress(Uint8);
	void handleRelease(Uint8);

private:
	Mouse();

	static bool instanceFlag;
	static Mouse *single;
};

struct Keyboard
{
	static Keyboard* getInstance();

	std::bitset<AMOUNT_OF_KEYS> keysPressed;
	std::bitset<AMOUNT_OF_KEYS> keysDown;
	std::bitset<AMOUNT_OF_KEYS> keysReleased;

	void reset();
	void handlePress(SDLKey);
	void handleRelease(SDLKey);

private:
	Keyboard();

	static bool instanceFlag;
	static Keyboard *single;
};

#endif
