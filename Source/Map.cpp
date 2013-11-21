#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;
const float Map::DEFAULT_SECTION_SPACING = 1000.0;



Map::Map(int seed, float scale)
	: SEED(seed), MAP_SCALE(scale) {
	NUMBER_OF_PLANETS = 2;
	MAX_NUMBER_OF_PLAYERS_PER_TEAM = int(6 * MAP_SCALE);
	MIN_NUMBER_OF_PLAYERS_PER_TEAM = 1;
	spawn_points = NULL;
}

Map::~Map() {
	if (spawn_points != NULL)
		delete[] spawn_points;
	Planet::Clear_Planets();
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
	map_bounds.x = (NUMBER_OF_PLANETS / 2) * MAP_SCALE * -1000.0 - 500.0 * MAP_SCALE;
	map_bounds.y = MAP_SCALE * -1000.0;
	map_bounds.w = NUMBER_OF_PLANETS * MAP_SCALE * 1000.0;
	map_bounds.h = MAP_SCALE * 2000.0;

	spawn_points = new float[MAX_NUMBER_OF_PLAYERS_PER_TEAM * 2];

	Planet::Generate_Planets(NUMBER_OF_PLANETS, MAP_SCALE);
}