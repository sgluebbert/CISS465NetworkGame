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


void Map::AddPlanet(int id, float x, float y)
{
	float size = 200.0f;
	PlanetState s = NEUTRAL;
	Planet * planet = new Planet(id, s, x, y, 0.0f, 1000.0f, 0.0f, size, 100.0f, 0.0f);
	planets.push_back(planet);
}

int Map::NumPlanets()
{
	return planets.size();
}

void Map::Update(double dt) {
}

int Map::AllPlanetsCaptured()
{
	bool blue_flag = 1;
	bool red_flag = 1;
	
	for (int i = 0; i < planets.size(); i++)
	{
		if (planets[i]->state != BLUE_PLANET)
			blue_flag =  0;

		if (planets[i]->state != RED_PLANET)
			red_flag =  0;
	}

	if (blue_flag == 1)
		return 1;

	if (red_flag == 1)
		return 2;

	return 0;	// Not all planets have been captured
}

void Map::PlanetCollision(Ship * ship)
{
	for (int i = 0; i < planets.size(); i++)
		planets[i]->UnderSiege(ship);
}


void Map::Draw(Camera * camera) {
}