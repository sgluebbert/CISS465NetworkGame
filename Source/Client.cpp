#include "Client.h"



Client::Client() {
	pawn = new Ship(FIGHTER, 0.0, 0.0);

	/*shield_bar.Set_Rect(720, 0, 80, 20, 2);
	shield_bar.Set_Color(Color(0.00, 0.00, 0.50));

	hull_bar.Set_Rect(720, 20, 80, 20, 2);
	hull_bar.Set_Color(Color(0.50, 0.00, 0.00));

	armor_bar.Set_Rect(720, 40, 80, 20, 2);
	armor_bar.Set_Color(Color(0.00, 0.50, 0.00));

	power_bar.Set_Rect(720, 60, 80, 20, 2);
	power_bar.Set_Color(Color(0.50, 0.50, 0.00));*/

	shield_circle.Set_Circle(32, 32, 32, 2, 16, 0);
	shield_circle.Set_Color(Color(0.00, 0.00, 0.50));

	hull_circle.Set_Circle(96, 32, 32, 2, 16, 0);
	hull_circle.Set_Color(Color(0.50, 0.00, 0.00));

	armor_circle.Set_Circle(96, 96, 32, 2, 16, 0);
	armor_circle.Set_Color(Color(0.00, 0.50, 0.00));

	power_circle.Set_Circle(32, 96, 32, 2, 16, 0);
	power_circle.Set_Color(Color(0.50, 0.50, 0.00));

	pawn->respawn_timer.Set_Interval(5.0);

	channel_id = -1;
	player_id = -1;
	team_id = -1;
	time(&last_input);

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;

	for (int i = 0; i < 5; ++i)
		fired[i] = false;

	offline = true;

	callsign = "Default";
}



Client::~Client() {
	delete pawn;
}



void Client::Respawn(double _x, double _y) {
	delete pawn;
	pawn = new Ship(FIGHTER, _x, _y);
}



void Client::Update(double dt) {
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
			fired[ENERGY_TYPE] = pawn->Fire(ENERGY_TYPE);
		if (inputs[FIRE_BALLISTIC])
			fired[BALLISTIC_TYPE] = pawn->Fire(BALLISTIC_TYPE);
		if (inputs[FIRE_PROPELLED])
			fired[PROPELLED_TYPE] = pawn->Fire(PROPELLED_TYPE);
		if (inputs[FIRE_MINE])
			fired[BOMB_TYPE] = pawn->Fire(BOMB_TYPE);
		if (inputs[FIRE_POWERUP])
			fired[POWERUP_TYPE] = pawn->Fire(POWERUP_TYPE);
	}

	/*armor_bar.Notify(pawn->armor / pawn->max_armor);
	hull_bar.Notify(pawn->health / pawn->max_health);
	shield_bar.Notify(pawn->shields / pawn->max_shields);
	power_bar.Notify(pawn->power / pawn->max_power);*/

	armor_circle.Notify(pawn->armor / pawn->max_armor);
	hull_circle.Notify(pawn->health / pawn->max_health);
	shield_circle.Notify(pawn->shields / pawn->max_shields);
	power_circle.Notify(pawn->power / pawn->max_power);

	pawn->Update(dt);

	if (pawn->state == DEAD && pawn->respawn_timer.Ended())
		Respawn(0.0, 0.0);

	/*armor_bar.Update(dt);
	hull_bar.Update(dt);
	shield_bar.Update(dt);
	power_bar.Update(dt);*/

	armor_circle.Update(dt);
	hull_circle.Update(dt);
	shield_circle.Update(dt);
	power_circle.Update(dt);
	info_feed.Update(dt);
}

void Client::Draw() {
	pawn->Draw();

	/*armor_bar.Draw();
	hull_bar.Draw();
	shield_bar.Draw();
	power_bar.Draw();*/

	armor_circle.Draw();
	hull_circle.Draw();
	shield_circle.Draw();
	power_circle.Draw();
	info_feed.Draw();
}