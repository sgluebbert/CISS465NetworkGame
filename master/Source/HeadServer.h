#ifndef HEADSERVER
#define HEADSERVER

#include "Lobby.h"
#include <deque>

struct HeadServer
{
	HeadServer();
	~HeadServer();

	bool Update();
	void Receive();
	
private:
	Network *network;
	std::deque<Lobby *> lobbies;
};

const std::string CurrentDateTime();

#endif