#ifndef CLIENT_H
#define CLIENT_H



#include <deque>

#include "./Networking/Network.h"
#include "Progress_Circle.h"
#include "Chat_Feed.h"
#include "Radar.h"
#include "Planet.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_PROPELLED, FIRE_MINE, FIRE_POWERUP, NUMBER_OF_INPUTS	};



class Client {
public:
	bool inputs[NUMBER_OF_INPUTS];
	char channel_id;
	char player_id; // Uint8  client server interaction uses a player id of -1 for init
	Team team_id;
	std::string player_name;
	time_t last_input;

	bool offline;

	Ship * pawn;

	Health_Bar armor_bar;
	Health_Bar hull_bar;
	Health_Bar shield_bar;
	Health_Bar power_bar;

	Progress_Bar weapon_status_bars[4];
	std::deque<Alignment_Bar> planet_alignment_bars;

	Radar radar;
	Chat_Feed info_feed;

	Client();
	~Client();

	void Respawn(double, double);

	void Update(double);
	void Draw();

private:
};



#endif