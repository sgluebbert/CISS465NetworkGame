#ifndef SERVER_H
#define SERVER_H

#include "../../Source/Networking/Network.h"
#include <ctime>
#include <vector>
#include <deque>
#include <bitset>
#include <string>


enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_MISSILE, FIRE_MINE, NUMBER_OF_INPUTS	};

struct ServerClient
{
	ServerClient(int _id = -1)
		: id(_id)
	{
		for (int i = 0; i < NUMBER_OF_INPUTS; ++i)
			inputs[i] = false;
	}
	time_t lastInput;
	int id;
	bool inputs[NUMBER_OF_INPUTS];
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
