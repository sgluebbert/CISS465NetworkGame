#ifndef PLANET_H
#define PLANET_H

#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Clock.h"
#include "Collidable.h"
#include "Progress_Bar.h"
#include "Drawing/Text.h"
#include "Collision.h"
#include "Ship.h"
#include <vector>
#include <string>
#include <sstream>

enum PlanetState { NEUTRAL, RED_PLANET, BLUE_PLANET };

class Planet : public Entity, public Collidable
{

public:

	Planet(int _id, PlanetState s, float _x, float _y, float _a, float m, float _v, float _r, float gr, float cv);
	~Planet();

	virtual void Update(double);
    virtual void Draw();

	void SetTexture(Texture *);
    void UnderSiege(Ship *);
    void DrawGravityField();

    int id;
    float r;
	Texture * texture;
	float capture_value;
	float gravity_radius;
	PlanetState state;
};

class PlanetsHUD
{

public:

	PlanetsHUD(int num);

	std::string int_to_string(int n)
	{
		std::stringstream ss;
		ss << n;
		return ss.str();
	}

	void Update(std::vector<Planet *> planets);
	void Draw();

	int num_planets;
	std::vector<Alignment_Bar *> bars;

};


#endif