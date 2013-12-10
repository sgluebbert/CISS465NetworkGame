#ifndef HEADSERVER
#define HEADSERVER

#include "../../Source/Networking/Lobby.h"
#include "../../Source/Database.h"
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <deque>

const int PortRangeStart = 1236;
const int AvailablePortsSize = 4;
const int LocalhostInt = 16777343; // Int representation of "localhost" in sdlnet

struct HeadServer
{
	HeadServer();
	~HeadServer();

	bool Update();
	void ReceivePlayers();
	void ReceiveLobbies();
	void NotifyPlayers(char id = -1);
	bool CreateLobby(std::string name, int port, float mapScale);

	bool AddLobby(Lobby &);
	bool SaveLobby(Lobby &);
	bool RemoveLobby(Lobby &);
	bool PerformLogin(std::string name, std::string password);
	
private:
	Network *networkLobbies;
	Network *networkPlayers;
	std::deque<Lobby *> lobbies;
	bool availablePorts[AvailablePortsSize];
	bool activePlayers[MaximumClients];
};

const std::string CurrentDateTime();

#endif