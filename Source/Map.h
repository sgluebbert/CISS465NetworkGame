#ifndef MAP_H
#define MAP_H



#include <vector>

#include "Asteroid.h"
#include "Moon.h"
#include "Planet.h"
#include "Powerup.h"
#include "Camera.h"
#include "Ship.h"

class Map {
public:
	static const int X_COORD;
	static const int Y_COORD;

	Map(int);
	~Map();

	const int MAX_NUMBER_OF_PLAYERS_PER_TEAM;
	const int MIN_NUMBER_OF_PLAYERS_PER_TEAM;
	const int SEED;
	const int NUMBER_OF_PLANETS;
	const float MAP_SCALE;
	
	Rect<int> map_bounds;
	float * spawn_points;

	std::vector<Moon *> moons;
	std::vector<Asteroid *> asteroids;
	std::vector<Powerup *> powerups;

	void Update(double);
	void Draw(Camera * camera);
	void DrawLobbyPreview(int x, int y, int w, int h);
};



#endif