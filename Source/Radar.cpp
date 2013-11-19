#include "Radar.h"



const int Radar::X_OFFSET = 350;
const int Radar::Y_OFFSET = 500;
const int Radar::BORDER_WIDTH = 2;



Radar::Radar() {
	bounding_box.x = 0;
	bounding_box.y = 0;
	bounding_box.w = 100;
	bounding_box.h = 100;
	radar_bounds.x = bounding_box.x + BORDER_WIDTH;
	radar_bounds.y = bounding_box.y + BORDER_WIDTH;
	radar_bounds.w = bounding_box.w - BORDER_WIDTH * 2;
	radar_bounds.h = bounding_box.h - BORDER_WIDTH * 2;
}

void Radar::Notify(std::deque<Ship *> & all_ships) {
	ships = all_ships;
}

void Radar::Notify(Ship * ship) {
	player_ship = ship;
}

void Radar::Draw() {
	if (player_ship == NULL)
		return;

	bounding_box.x += X_OFFSET;
	bounding_box.y += Y_OFFSET;
	radar_bounds.x += X_OFFSET;
	radar_bounds.y += Y_OFFSET;

	DrawRect(bounding_box.x, bounding_box.y, bounding_box.x + bounding_box.w, bounding_box.y + bounding_box.h, true, &WHITE);
	DrawRect(radar_bounds.x, radar_bounds.y, radar_bounds.x + radar_bounds.w, radar_bounds.y + radar_bounds.h, true, &BLACK);

	for (int i = 0; i < ships.size(); i++) {
		if (ships[i] == NULL)
			continue;

		int _x, _y, _r = 1;
		_x = radar_bounds.x + (ROOM_WIDTH / ships[i]->x) * radar_bounds.w;
		_y = radar_bounds.y + (ROOM_HEIGHT / ships[i]->y) * radar_bounds.h;

		if (ships[i] == player_ship)
			DrawCircle(_x, _y, _r, true, &LIGHT_GREEN);
		else if (ships[i]->team_id == player_ship->team_id)
			DrawCircle(_x, _y, _r, true, &LIGHT_BLUE);
		else //if (ships[i]->team_id != player_ship->team_id)
			DrawCircle(_x, _y, _r, true, &LIGHT_RED);
	}

	bounding_box.x -= X_OFFSET;
	bounding_box.y -= Y_OFFSET;
	radar_bounds.x -= X_OFFSET;
	radar_bounds.y -= Y_OFFSET;
}