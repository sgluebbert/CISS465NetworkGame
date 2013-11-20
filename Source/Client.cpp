#include "Client.h"



Client::Client() {
	pawn = NULL;

	shield_bar.Set_Rect(0, 0, 80, 20, 2);
	shield_bar.Set_Color(Color(0.00, 0.00, 0.50));

	hull_bar.Set_Rect(0, 20, 80, 20, 2);
	hull_bar.Set_Color(Color(0.50, 0.00, 0.00));

	armor_bar.Set_Rect(0, 40, 80, 20, 2);
	armor_bar.Set_Color(Color(0.00, 0.50, 0.00));

	power_bar.Set_Rect(0, 60, 80, 20, 2);
	power_bar.Set_Color(Color(0.50, 0.50, 0.00));

	channel_id = -1;
	player_id = -1;
	team_id = NO_TEAM;
	time(&last_input);

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;

	offline = true;

	player_name	 = "Default";
}



Client::~Client() {
	if (pawn != NULL)
		delete pawn;
}



void Client::Respawn(double _x, double _y) {
	if (pawn != NULL)
		delete pawn;
	pawn = new Ship(INTERCEPTOR, _x, _y);
}



void Client::Update(double dt) {
	if (pawn != NULL) {
 		if (offline) {
 			if (inputs[MOVE_FORWARD])
 				pawn->Accelerate(false);
 			else if (inputs[MOVE_BACKWARD])
 				pawn->Accelerate(true);
 			else
 				pawn->Decelerate();
 
 			if (inputs[TURN_LEFT])
 				pawn->Turn_Left(dt);
 			if (inputs[TURN_RIGHT])
 				pawn->Turn_Right(dt);
 
 			if (inputs[FIRE_ENERGY])
 				pawn->Fire(ENERGY_TYPE);
 			if (inputs[FIRE_BALLISTIC])
 				pawn->Fire(BALLISTIC_TYPE);
 			if (inputs[FIRE_PROPELLED])
 				pawn->Fire(PROPELLED_TYPE);
 			if (inputs[FIRE_MINE])
 				pawn->Fire(BOMB_TYPE);
 			if (inputs[FIRE_POWERUP])
 				pawn->Fire(POWERUP_TYPE);
 		}

		armor_bar.Notify(pawn->armor / pawn->max_armor);
		hull_bar.Notify(pawn->health / pawn->max_health);
		shield_bar.Notify(pawn->shields / pawn->max_shields);
		power_bar.Notify(pawn->power / pawn->max_power);

		if (pawn->state == DEAD && pawn->respawn_timer.Ended())
			Respawn(0.0, 0.0);
	}

	armor_bar.Update(dt);
	hull_bar.Update(dt);
	shield_bar.Update(dt);
	power_bar.Update(dt);
	info_feed.Update(dt);
}

void Client::Draw() {
	armor_bar.Draw();
	hull_bar.Draw();
	shield_bar.Draw();
	power_bar.Draw();
	info_feed.Draw();
}