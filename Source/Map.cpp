#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;



Map::Map(int seed)
	: SEED(seed), WIDTH(4000), HEIGHT(2000), MAX_NUMBER_OF_PLAYERS(32) {
	spawn_points = new float[MAX_NUMBER_OF_PLAYERS * 2];
}

Map::~Map() {
	delete[] spawn_points;
}



void Map::Update(double dt) {
	//Update all items
}

void Map::Draw() {
	//Draw all items
}