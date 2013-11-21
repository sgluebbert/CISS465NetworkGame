#include <iostream>
#include "./Source/HeadServer.h"

int main(int argc, char const *argv[])
{
	HeadServer server;
	while (!server.Update())
	{
		// We delay any decent amount of time since we don't need instant responses.
		// We also don't want to waste cpu
		SDL_Delay(60);
	}

	return 0;
}