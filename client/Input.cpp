#include "Input.h"

bool Mouse::instanceFlag = false;
Mouse* Mouse::single = NULL;

Mouse::Mouse()
{
	leftPressed = false; leftDown = false; leftReleased = false;
	rightPressed = false; rightDown = false; rightReleased = false;
}

Mouse* Mouse::getInstance()
{
	if (!instanceFlag)
	{
		single = new Mouse();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}

void Mouse::reset()
{
	leftPressed = false;
	leftReleased = false;
	rightPressed = false;
	rightReleased = false;
}

void Mouse::handlePress(Uint8 button)
{
	switch(button)
	{
	case SDL_BUTTON_LEFT:
		leftPressed = true;
		leftDown = true;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT: 
		rightPressed = true;
		rightDown = true;
		break;
	case SDL_BUTTON_WHEELUP:
		break;
	case SDL_BUTTON_WHEELDOWN:
		break;
	}
}

void Mouse::handleRelease(Uint8 button)
{
	switch(button)
	{
	case SDL_BUTTON_LEFT:
		leftPressed = false;
		leftDown = false;
		leftReleased = true;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT: 
		rightPressed = false;
		rightDown = false;
		rightReleased = true;
		break;
	case SDL_BUTTON_WHEELUP:
		break;
	case SDL_BUTTON_WHEELDOWN:
		break;
	}
}
