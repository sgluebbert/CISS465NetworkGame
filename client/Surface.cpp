#include "Surface.h"

bool Window::instanceFlag = false;
Window* Window::single = NULL;

Window::Window()
{
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE );
	SDL_WM_SetCaption("Not Named", NULL);
	surface = SDL_SetVideoMode ( WIDTH, HEIGHT, BPP, FLAGS );

	if (surface == NULL)
	{
		printf("Unable to set %ix%i video: %s\n", WIDTH, HEIGHT, SDL_GetError());
		fflush(stdout);
		exit(1);
	}
	atexit( SDL_Quit );
}

Window* Window::getInstance()
{
	if (!instanceFlag)
	{
		single = new Window();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}
