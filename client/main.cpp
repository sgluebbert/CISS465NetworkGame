#include <iostream>
#include "Surface.h"
#include "Input.h"

int main(int argc, char const *argv[])
{
	Window* window = Window::getInstance();
	Mouse* mouse = Mouse::getInstance();

	SDL_Event event;
	bool running = true;
	while (running)
	{
		mouse->reset();
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT: return 0;
				case SDL_MOUSEBUTTONDOWN:
					mouse->handlePress(event.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					mouse->handleRelease(event.button.button);
					break;
				case SDL_MOUSEMOTION:
					mouse->dx = event.motion.xrel;
					mouse->dy = event.motion.yrel;
					mouse->x = event.motion.x;
					mouse->y = event.motion.y;
					break;
				case SDL_KEYDOWN: 
					// keyboard.pressed(event.key.keysym.sym);
					break;
				case SDL_KEYUP: 
					// keyboard.released(event.key.keysym.sym);
					break;
			}
		}
	}

	return 0;
}
