#ifndef CLIENT_H
#define CLIENT_H



#include "Ship.h"
#include "./Networking/Network.h"
#include "Progress_Circle.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_PROPELLED, FIRE_MINE, FIRE_POWERUP, NUMBER_OF_INPUTS	};



class Client {
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

	// Health_Bar armor_bar;
	// Health_Bar hull_bar;
	// Health_Bar shield_bar;
	// Health_Bar power_bar;

	Health_Circle armor_circle;
	Health_Circle hull_circle;
	Health_Circle shield_circle;
	Health_Circle power_circle;

	Client();
	~Client();

	void Respawn(double, double);

	void Update(double);
	void Draw();

	bool Connect();
	bool Send();
	NetString * Receive();

private:
};



#endif