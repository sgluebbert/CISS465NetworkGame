#ifndef CLIENT_H
#define CLIENT_H



#include "Ship.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_MISSILE, FIRE_MINE, NUMBER_OF_INPUTS	};



class Client {
public:
	Uint8 inputs[NUMBER_OF_INPUTS];
	Uint8 channel_id;
	Uint8 player_id;
	Uint8 team_id;
	Uint16 host_port;

	Ship * pawn;

	Client();

	void Update(double);
	void Draw();

	void Send();
	void Receive();

private:
};



#endif