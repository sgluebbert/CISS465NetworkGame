#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;
const float Map::DEFAULT_SECTION_SPACING = 1000.0;



Map::Map(int seed, float scale)
	: SEED(seed) {
	srand(SEED);

	if (scale < 0.5)
		map_scale = 0.5;
	else if (scale > 2.5)
		map_scale = 2.5;
	else
		map_scale = scale;

	max_players_per_team = int(6 * map_scale);
	min_players_per_team = 1;
	number_of_planets = rand() % 7 + 1;

	map_bounds.x = (number_of_planets / 2) * map_scale * -1000.0 - 500.0 * map_scale;
	map_bounds.y = map_scale * -1000.0;
	map_bounds.w = number_of_planets * map_scale * 1000.0;
	map_bounds.h = map_scale * 2000.0;

	srand(time(NULL));
}

Map::~Map() {
	Planet::Clear_Planets();

	for (int i = 0; i < pickups.size(); i++)
		delete pickups[i];
}

void Map::Update(double dt) {
}


void Map::Draw() {
}

void Map::DrawLobbyPreview(int x, int y, int w, int h) {
	// Draws a mini map for use in the lobby.  Should put everything in the rectangle given.
	// We could center it within the rectangle if the rectangle is not the correct aspect ratio.
}

void Map::Generate_Map() {
	srand(SEED);
	Planet::Generate_Planets(number_of_planets, map_scale);

	for (int i = -1; i < 2; i += 2) {
		Pickup_Factory * temp = new Pickup_Factory(0, i * 250);

		temp->Set_Pickup(new Collectible());

		pickups.push_back(temp);
	}
	
	srand(time(NULL));
}