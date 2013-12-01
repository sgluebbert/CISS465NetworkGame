#ifndef HEADSERVER
#define HEADSERVER

#include "../../Source/Networking/Lobby.h"
#include <deque>

struct HeadServer
{
	HeadServer();
	~HeadServer();

	bool Update();
	void ReceivePlayers();
	void ReceiveLobbies();
	void NotifyPlayers(char id = -1);
	
private:
	Network *networkLobbies;
	Network *networkPlayers;
	std::deque<Lobby *> lobbies;
	bool activePlayers[MaximumClients];
};

const std::string CurrentDateTime();

#endif