#ifndef SURFACE_H
#define SURFACE_H

#include "SDL.h"
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;
const int BPP = 0;
const int FLAGS = SDL_ANYFORMAT;

struct Window
{
	static Window* getInstance();
	bool checkStable() { return stable; }

private:
	Window();

	SDL_Surface *surface;
	bool stable;

	static bool instanceFlag;
	static Window *single;
};

#endif
