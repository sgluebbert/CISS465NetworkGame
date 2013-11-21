#ifndef PLANET_H
#define PLANET_H



#include <list>

#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "Collidable.h"
#include "Collision.h"
#include "Ship.h"



class Planet : public Entity, public Collidable {
public:
	virtual void Update(double);
    virtual void Draw();

	Texture * field;
	float alignment;
	float capture_rate;
	float gravity_radius;
	bool locked;

	static std::list<Planet *> planet_graph;
	static float field_modifier;

	static void Generate_Planets(int, float);
	static void Clear_Planets();
	static Team Win_Condition();

private:
	Planet(Team, float, float, float, float, float);
	~Planet();
};



#endif