#ifndef SERVER_H
#define SERVER_H

#include "Network.h"
#include <ctime>
#include <vector>
#include <deque>
#include <bitset>
#include <string>

const int MaximumClients = 32;

struct ServerClient
{
	ServerClient(Uint64 _ip, Uint32 _port, int _channel = -1)
		: ip(_ip), port(_port), lastInput(0), channel(_channel)
	{
		inputs[0] = 0; inputs[1] = 0; inputs[2] = 0; inputs[3] = 0;
	}
	Uint64 ip;
	Uint32 port;
	time_t lastInput;
	int channel;
	bool inputs[4];
};

enum DebugLevel { DL_NONE, DL_LOW, DL_MED, DL_HIGH };

struct Server
{
	Server(DebugLevel l = DL_MED);
	~Server();

	std::vector<int> HandleIncomingData();
	std::bitset<MaximumClients> CleanupClients();
	void SendToAll(NetString *string);
	ServerClient* clientList[MaximumClients];

private:
	int GetNextChannel();

	void CheckSpeed();
	int frameCount;
	time_t lastSpeedDisplay;

	int clientCount;
	int checkForInactivity;
	DebugLevel debugLevel;
	std::deque<ServerClient *> clients;
	std::bitset<MaximumClients> usedChannels;
	Network *network;
};

const std::string CurrentDateTime();

#endif
