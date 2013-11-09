#ifndef PLAYER_H
#define PLAYER_H



#include "Ship.h"
#include "./Networking/Network.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_MISSILE, FIRE_MINE, NUMBER_OF_INPUTS	};



class Player {
public:
	bool inputs[NUMBER_OF_INPUTS];
	char channel_id;
	Uint8 player_id;
	Uint8 team_id;
	Uint16 host_port;

	Network * network;
	NetString netString;

	int ready;
	bool offline;

	Ship * pawn;

	Player();
	~Player();

	void Update(double);
	void Draw();

	bool Send();
	NetString * Receive();

private:
};



#endif