#ifndef SERVER_H
#define SERVER_H

#include "../../Source/Networking/Network.h"
#include <ctime>
#include <vector>
#include <deque>
#include <bitset>
#include <string>

struct ServerClient
{
	ServerClient(int _id = -1)
		: id(_id)
	{
		inputs[0] = 0; inputs[1] = 0; inputs[2] = 0; inputs[3] = 0;
	}
	time_t lastInput;
	int id;
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
	void CheckSpeed();
	int frameCount;
	time_t lastSpeedDisplay;

	int clientCount;
	int checkForInactivity;
	DebugLevel debugLevel;
	std::vector<int> removedClients;
	Network *network;
};

const std::string CurrentDateTime();

#endif
