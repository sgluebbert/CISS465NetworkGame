#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;



Map::Map(int seed)
	: SEED(seed), NUMBER_OF_PLANETS(5), MAP_SCALE(1.0), MAX_NUMBER_OF_PLAYERS_PER_TEAM(10), MIN_NUMBER_OF_PLAYERS_PER_TEAM(1) {
	map_bounds.x = (NUMBER_OF_PLANETS / 2) * MAP_SCALE * -1000.0 - 500.0 * MAP_SCALE;
	map_bounds.y = MAP_SCALE * -1000.0;
	map_bounds.w = NUMBER_OF_PLANETS * MAP_SCALE * 1000.0;
	map_bounds.h = MAP_SCALE * 2000.0;
	spawn_points = new float[MAX_NUMBER_OF_PLAYERS_PER_TEAM * 2];
	Planet::Generate_Planets(NUMBER_OF_PLANETS, MAP_SCALE);
}

Map::~Map() {
	delete[] spawn_points;
	Planet::Clear_Planets();
}

void Map::Update(double dt) {
}


void Map::Draw(Camera * camera) {
}

void Map::DrawLobbyPreview(int x, int y, int w, int h) {
	// Draws a mini map for use in the lobby.  Should put everything in the rectangle given.
	// We could center it within the rectangle if the rectangle is not the correct aspect ratio.
}