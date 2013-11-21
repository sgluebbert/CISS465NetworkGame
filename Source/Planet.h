#ifndef PLANET_H
#define PLANET_H



#include <list>
#include <vector>

#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "Collidable.h"
#include "Collision.h"
#include "Ship.h"
#include "Moon.h"


class Planet : public Entity, public Collidable
{

public:

	Planet(Team _id, float _x, float _y, float _m, float _r);
	~Planet();

	virtual void Update(double);
    virtual void Draw();
	void Generate_Moons();

    void UnderSiege(Ship *);
    void DrawGravityField();

	Texture * field;
	float capture_value;
	float gravity_radius;
	bool locked;

	std::vector<Moon *> moons;

	//Best way to allow planets to manage themselves unfortunately
	//May be better to add planets from map rather than within this so there is no need to keep the planet vector, but meh
	static std::list<Planet *> planet_graph;
	static float field_modifier;

	static void Generate_Planets(int);
	static void Clear_Planets();
	static Team Win_Condition();
};



#endif