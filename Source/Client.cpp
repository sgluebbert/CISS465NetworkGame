#include "Client.h"



Client::Client() {
	pawn = NULL;

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
	Ship::Remove_Ship(player_id);
	pawn = NULL;
}



void Client::Respawn(int spawnIndex) {
	float _x = 0, _y = 0;
	if (spawnPoints.size() != 0)
	{
		SpawnPoint spawnPoint;
		if (spawnIndex == -1)
			spawnPoint = spawnPoints[rand() % spawnPoints.size()];
		else
			spawnPoint = spawnPoints[spawnIndex % spawnPoints.size()];
		_x = spawnPoint.x + rand() % 80 - 40;
		_y = spawnPoint.y + rand() % 80 - 40;
	}

	pawn->dx = pawn->dy = pawn->force = pawn->torque = pawn->velocity = pawn->rotation = 0.0;

	pawn->x = pawn->bounding_volume.x = _x;
	pawn->y = pawn->bounding_volume.y = _y;
	if (pawn->team_id == RED_TEAM)
		pawn->draw_angle = pawn->angle = 0.0 + rand() % 20 - 10;
	else
		pawn->draw_angle = pawn->angle = -180 + rand() % 20 - 10;
	pawn->drawing_box.x = pawn->x - pawn->bounding_volume.r;
	pawn->drawing_box.y = pawn->y - pawn->bounding_volume.r;

	pawn->state = ALIVE;

	pawn->health = pawn->max_health;
	pawn->shields = pawn->max_shields;
	pawn->armor = pawn->max_armor;
	pawn->power = pawn->max_power;

	pawn->weapon_pool[ENERGY_TYPE]	= new Laser(pawn);
	pawn->weapon_pool[BALLISTIC_TYPE] = new Gauss(pawn);
	pawn->weapon_pool[PROPELLED_TYPE] = new Rocket(pawn);
	pawn->weapon_pool[BOMB_TYPE]		= new Bomb(pawn);
	pawn->weapon_pool[POWERUP_TYPE]	= NULL;
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
 				if (pawn->Fire(ENERGY_TYPE))
 					stats.shotsFired++;
 			if (inputs[FIRE_BALLISTIC])
 				if (pawn->Fire(BALLISTIC_TYPE))
 					stats.shotsFired++;
 			if (inputs[FIRE_PROPELLED])
 				if (pawn->Fire(PROPELLED_TYPE))
 					stats.shotsFired++;
 			if (inputs[FIRE_MINE])
 				if (pawn->Fire(BOMB_TYPE))
 					stats.shotsFired++;
 			if (inputs[FIRE_POWERUP])
 				if (pawn->Fire(POWERUP_TYPE))
 					stats.shotsFired++;
 		}

		if (pawn->state == DYING)
			stats.totalDeaths++;
		if (pawn->state == DEAD && pawn->respawn_timer.Ended())
			Respawn();
		stats.captures += pawn->captures;
		pawn->captures = 0;
		stats.shotsHit += pawn->shots_hit;
		pawn->shots_hit = 0;
		stats.totalKills += pawn->kills;
		pawn->kills = 0;
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


	for (int i = 0; i < planet_alignment_bars.size(); i++)
		planet_alignment_bars[i].Update(dt);

	info_feed.Update(dt);
}

void Client::Draw() {

	for (int i = 0; i < planet_alignment_bars.size(); i++)
		planet_alignment_bars[i].Draw();

	info_feed.Draw();
}
