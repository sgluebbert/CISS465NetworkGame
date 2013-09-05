#include <iostream>
#include "Surface.h"

int main(int argc, char const *argv[])
{
	Window* window = Window::getInstance();
	if (!window->checkStable())
	{
		return 0;
	}

	SDL_Event event;
	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT: return 0;
			}
		}
	}

	return 0;
}
