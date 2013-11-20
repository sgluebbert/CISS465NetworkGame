#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;



Map::Map(int seed)
	: SEED(seed), WIDTH(4000), HEIGHT(2000), MAX_NUMBER_OF_PLAYERS_PER_TEAM(10), MIN_NUMBER_OF_PLAYERS_PER_TEAM(1) {
	spawn_points = new float[MAX_NUMBER_OF_PLAYERS_PER_TEAM * 2];
}

Map::~Map() {
	delete[] spawn_points;

    while (!Planet::planet_graph.empty()) {
    	delete Planet::planet_graph.front();
    	Planet::planet_graph.pop_front();
    }

	Planet::planet_graph.clear();
}


void Map::AddPlanet(Team id, float x, float y)
{
	float size = 200.0f;
	float alignment;

	if (id == RED_TEAM)
		alignment = -1.0;
	else if (id == BLUE_TEAM)
		alignment = 1.0;
	else
		alignment = 0.0;

	Planet * planet = new Planet(id, x, y, 0.0f, 1000.0f, 0.0f, size, 100.0f, alignment);
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
		if (planets[i]->team_id != BLUE_TEAM)
			blue_flag =  0;

		if (planets[i]->team_id != RED_TEAM)
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

void Map::DrawLobbyPreview(int x, int y, int w, int h) {
	// Draws a mini map for use in the lobby.  Should put everything in the rectangle given.
	// We could center it within the rectangle if the rectangle is not the correct aspect ratio.
}