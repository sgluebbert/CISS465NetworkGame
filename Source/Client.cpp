#include "Client.h"



Client::Client() {
	pawn = new Fighter(0.0, 0.0);

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = 0;

	channel_id = -1;
	player_id = -1;
	team_id = -1;

	host_port = 8080;
}



void Client::Send() {

}

void Client::Receive() {

}



void Client::Update(double dt) {
	if (inputs[MOVE_FORWARD])
		pawn->force = 200.0;
	else
		pawn->force = 0.0;
	if (inputs[MOVE_BACKWARD])
		pawn->force = -200.0;
	else
		pawn->force = 0.0;
	if (inputs[TURN_LEFT])
		pawn->Turn_Left(dt);
	if (inputs[TURN_RIGHT])
		pawn->Turn_Right(dt);
	if (inputs[FIRE_ENERGY])
		pawn->Fire(0);
	if (inputs[FIRE_BALLISTIC])
		pawn->Fire(1);
	if (inputs[FIRE_MISSILE])
		pawn->Fire(2);
	if (inputs[FIRE_MINE])
		pawn->Fire(3);

	pawn->Update(dt);
}

void Client::Draw() {
	pawn->Draw();
}