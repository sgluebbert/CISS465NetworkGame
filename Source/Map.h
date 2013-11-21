#ifndef MAP_H
#define MAP_H



#include <vector>

#include "Asteroid.h"
#include "Moon.h"
#include "Planet.h"
#include "Camera.h"
#include "Ship.h"

class Map {
public:
	static const int X_COORD;
	static const int Y_COORD;
	static const float DEFAULT_SECTION_SPACING;

	Map(int, float);
	~Map();

	const int SEED;
	const float MAP_SCALE;

	int MAX_NUMBER_OF_PLAYERS_PER_TEAM;
	int MIN_NUMBER_OF_PLAYERS_PER_TEAM;
	int NUMBER_OF_PLANETS;

	Rect<int> map_bounds;
	float * spawn_points;

	std::vector<Moon *> moons;
	std::vector<Asteroid *> asteroids;

	void Update(double);
	void Draw();

	void DrawLobbyPreview(int x, int y, int w, int h);
	void Generate_Map();
};



#endif