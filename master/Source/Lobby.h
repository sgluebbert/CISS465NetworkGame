#ifndef LOBBY
#define LOBBY

#include "../../Source/Networking/Network.h"
#include <string>

struct Lobby
{
	char channelId;
	std::string name;
	int mapSeed;
	Uint16 gamePort;
	IPaddress address;
	GameServerEnums state;
	unsigned char playerCount;
};

#endif
