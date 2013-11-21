#ifndef PLANET_H
#define PLANET_H



#include <list>

#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "Collidable.h"
#include "Collision.h"
#include "Ship.h"



class Planet : public Entity, public Collidable
{

public:

	Planet(Team, float, float, float, float, float);
	~Planet();

	virtual void Update(double);
    virtual void Draw();

    void UnderSiege(Ship *);
    void DrawGravityField();

	Texture * field;
	float alignment;
	float capture_rate;
	float gravity_radius;
	bool locked;

	//Best way to allow planets to manage themselves unfortunately
	//May be better to add planets from map rather than within this so there is no need to keep the planet vector, but meh
	static std::list<Planet *> planet_graph;
	static float field_modifier;

	static void Generate_Planets(int, float);
	static void Clear_Planets();
	static Team Win_Condition();
};



#endif