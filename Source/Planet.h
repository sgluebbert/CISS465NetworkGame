#ifndef PLANET_H
#define PLANET_H



#include <vector>
#include <string>
#include <sstream>
#include <list>

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



class Planet : public Entity, public Collidable
{

public:

	Planet(Team _id, float _x, float _y, float _a, float m, float _v, float _r, float gr, float cv);
	~Planet();

	virtual void Update(double);
    virtual void Draw();

    void UnderSiege(Ship *);
    void DrawGravityField();

	Texture * field;
	float capture_value;
	float gravity_radius;
	bool locked;

	//Best way to allow planets to manage themselves unfortunately
	//May be better to add planets from map rather than within this so there is no need to keep the planet vector, but meh
	static std::list<Planet *> planet_graph;
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