#include "Client.h"



Client::Client() {
	pawn = NULL;

	shield_bar.Set_Rect(0, 0, 100, 20, 2);
	shield_bar.Set_Color(Color(0.00, 0.00, 0.50));

	hull_bar.Set_Rect(0, 20, 100, 20, 2);
	hull_bar.Set_Color(Color(0.50, 0.00, 0.00));

	armor_bar.Set_Rect(0, 40, 100, 20, 2);
	armor_bar.Set_Color(Color(0.00, 0.50, 0.00));

	power_bar.Set_Rect(0, 60, 100, 20, 2);
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
		
		radar.Notify(pawn);

		if (pawn->state == DEAD && pawn->respawn_timer.Ended())
			Respawn(0.0, 0.0);
	}

	while (planet_alignment_bars.size() < Planet::planet_graph.size()) {
		planet_alignment_bars.push_back(Alignment_Bar());
        //planet_alignment_bars.back().progress = 0.0f;
        planet_alignment_bars.back().lhs_color = Color(0.5f, 0.0f, 0.0f, 1.0f);
        planet_alignment_bars.back().rhs_color = Color(0.0f, 0.0f, 0.5f, 1.0f);
	}

	{
		int i = 0;
	    for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it) {
	    	planet_alignment_bars[i].Set_Rect(0, 580 - 20 * i, 100, 20, 2);
	    	planet_alignment_bars[i++].Notify((*it)->alignment);
	    }
	}

	armor_bar.Update(dt);
	hull_bar.Update(dt);
	shield_bar.Update(dt);
	power_bar.Update(dt);

	for (int i = 0; i < planet_alignment_bars.size(); i++)
		planet_alignment_bars[i].Update(dt);

	info_feed.Update(dt);
}

void Client::Draw() {
	armor_bar.Draw();
	hull_bar.Draw();
	shield_bar.Draw();
	power_bar.Draw();

	for (int i = 0; i < planet_alignment_bars.size(); i++)
		planet_alignment_bars[i].Draw();

	radar.Draw();
	info_feed.Draw();
}