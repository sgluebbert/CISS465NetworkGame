#include "Input.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Mouse

bool Mouse::instanceFlag = false;
Mouse* Mouse::single = NULL;

Mouse::Mouse()
{
	leftPressed = false; leftDown = false; leftReleased = false;
	rightPressed = false; rightDown = false; rightReleased = false;
	x = 0; y = 0; dx = 0; dy = 0;
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
	dx = 0;
	dy = 0;
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

////////////////////////////////////////////////////////////////////////////////////////////
// Keyboard

bool Keyboard::instanceFlag = false;
Keyboard* Keyboard::single = NULL;

Keyboard::Keyboard()
{
}

Keyboard* Keyboard::getInstance()
{
	if (!instanceFlag)
	{
		single = new Keyboard();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}

void Keyboard::reset()
{
	keysPressed.reset();
	keysReleased.reset();
}

void Keyboard::handlePress(SDLKey key)
{
	int index = (int)key;
	if (index >= AMOUNT_OF_KEYS) return;
	keysPressed.set(index, true);
	keysDown.set(index, true);
}

void Keyboard::handleRelease(SDLKey key)
{
	int index = (int)key;
	if (index >= AMOUNT_OF_KEYS) return;
	keysReleased.set(index, true);
	keysDown.set(index, false);
}
