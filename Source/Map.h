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
	const int WIDTH, HEIGHT;
	const int SEED;

	float * spawn_points;

	std::vector<Planet *> planets;
	std::vector<Moon *> moons;
	std::vector<Asteroid *> asteroids;
	std::vector<Powerup *> powerups;


	int AllPlanetsCaptured();
	void PlanetCollision(Ship *);
	void AddPlanet(int id, float x, float y);
	int NumPlanets();
	void Update(double);
	void Draw(Camera * camera);
};



#endif