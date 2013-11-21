#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;



Map::Map(int seed)
	: SEED(seed), WIDTH(4000), HEIGHT(2000), MAX_NUMBER_OF_PLAYERS_PER_TEAM(10), MIN_NUMBER_OF_PLAYERS_PER_TEAM(1) {
	number_of_planets = 5;
	map_scale = 1.0;
	spawn_points = new float[MAX_NUMBER_OF_PLAYERS_PER_TEAM * 2];
	Planet::Generate_Planets(number_of_planets, map_scale);
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