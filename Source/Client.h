#ifndef CLIENT_H
#define CLIENT_H



#include "Ship.h"
#include "./Networking/Network.h"
#include "Progress_Circle.h"
#include "Chat_Feed.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_PROPELLED, FIRE_MINE, FIRE_POWERUP, NUMBER_OF_INPUTS	};



class Client {
public:
	bool inputs[NUMBER_OF_INPUTS];
	char channel_id;
	Uint8 player_id;
	Uint8 team_id; //<---------Check Entity, this should be dumped in favor of the enum
	std::string player_name;
	time_t last_input;

	Network * network;
	NetString netString;

	bool offline;

	Ship * pawn;

	Health_Bar armor_bar;
	Health_Bar hull_bar;
	Health_Bar shield_bar;
	Health_Bar power_bar;

	Chat_Feed info_feed;

	Client();
	~Client();

	void Respawn(double, double);

	void Update(double);
	void Draw();

private:
};



#endif