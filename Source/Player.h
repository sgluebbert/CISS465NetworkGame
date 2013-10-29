#ifndef PLAYER_H
#define PLAYER_H



#include "Ship.h"



enum INPUTS {	MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE_ENERGY, FIRE_BALLISTIC, FIRE_MISSILE, FIRE_MINE, NUMBER_OF_INPUTS	};



class Player {
public:
	Player();

	void Update(double);
	void Draw();

private:
	bool inputs[NUMBER_OF_INPUTS];
	Ship * pawn;
};



Player::Player() {
	pawn = new Fighter();

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;
}



Player::Update(double dt) {
	if (input[MOVE_FORWARD])
		pawn->Set_Force();
	if (input[MOVE_BACKWARD])
		pawn->Set_Force();
	if (input[TURN_LEFT])
		pawn->Turn_Left();
	if (input[TURN_RIGHT])
		pawn->Turn_Right();
	if (input[FIRE_ENERGY])
		pawn->Fire(0);
	if (input[FIRE_BALLISTIC])
		pawn->Fire(1);
	if (input[FIRE_MISSILE])
		pawn->Fire(2);
	if (input[FIRE_MINE])
		pawn->Fire(3);
}



#endif