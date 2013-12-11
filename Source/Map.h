#ifndef MAP_H
#define MAP_H



#include "Asteroid.h"
#include "Moon.h"
#include "Planet.h"
#include "Camera.h"
#include "Pickup_Factory.h"
#include "Ship.h"



class Map {
public:
	static const int X_COORD;
	static const int Y_COORD;
	static const float DEFAULT_SECTION_SPACING;

	Map(int, float);
	~Map();

	const int SEED;
	
	float map_scale;
	int max_players_per_team;
	int min_players_per_team;
	int number_of_planets;

	Rect<int> map_bounds;

	std::deque<Pickup_Factory *> pickups;
	std::vector<SpawnPoint> red_spawn_points;
	std::vector<SpawnPoint> blue_spawn_points;

	void Update(double);
	void Draw();

	void DrawLobbyPreview(int x, int y, int w, int h);
	void Generate_Map();
};



#endif