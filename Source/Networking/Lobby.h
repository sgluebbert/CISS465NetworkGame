#ifndef LOBBY
#define LOBBY

#include "Network.h"
#include <string>

struct Lobby
{
	char channelId;
	std::string name;
	int mapSeed;
	float mapScale;
	Uint16 gamePort; // broadcasted port for lobby's game
	IPaddress address; // address and port of lobby connected to master server
	GameServerEnums state;
	unsigned char playerCount;

	Lobby()
		: channelId(-1), name("New Lobby"), mapSeed(-1), mapScale(1), gamePort(0), state(GSE_WAITING), playerCount(0)
	{
		address.host = 0;
		address.port = 0;
	}

	NetString * Serialize()
	{
		NetString *string = new NetString();
		string->WriteUChar((unsigned char)channelId);
		string->WriteString(name);
		string->WriteInt(mapSeed);
		string->WriteFloat(mapScale);
		string->WriteInt((int)gamePort);
		string->WriteInt((int)address.host);
		string->WriteUChar((unsigned char)state);
		string->WriteUChar(playerCount);
		return string;
	}

	bool Deserialize(NetString *string)
	{
		unsigned char temp;
		if (!string->ReadUChar(temp) || !string->ReadString(name) || !string->ReadInt(mapSeed) || !string->ReadFloat(mapScale))
			return false;

		channelId = temp;

		int port, add;
		if (!string->ReadInt(port) || !string->ReadInt(add))
			return false;

		gamePort = port;
		address.host = add;

		if (!string->ReadUChar(temp) || !string->ReadUChar(playerCount))
			return false;

		state = (GameServerEnums)temp;

		return true;
	}
};

#endif
