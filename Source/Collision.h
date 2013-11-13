#ifndef COLLISION_H
#define COLLISION_H



#include "Collidable.h"
#include "Entity.h"


class Collision {
public:
	Collision();
	
	float x, y;		//Point of contact
	Collidable * lhs;
	Collidable * rhs;

	bool DoCollide(Entity lhs, Entity rhs);
};

static Collision * Collides(Collidable *, Collidable *);



#endif