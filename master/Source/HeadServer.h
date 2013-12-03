#ifndef HEADSERVER
#define HEADSERVER

#include "../../Source/Networking/Lobby.h"
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <deque>

struct HeadServer
{
	HeadServer();
	~HeadServer();

	bool Update();
	void ReceivePlayers();
	void ReceiveLobbies();
	void NotifyPlayers(char id = -1);
	bool CreateLobby(std::string name, int port, float mapScale);
	
private:
	Network *networkLobbies;
	Network *networkPlayers;
	std::deque<Lobby *> lobbies;
	bool activePlayers[MaximumClients];
};

const std::string CurrentDateTime();

#endif