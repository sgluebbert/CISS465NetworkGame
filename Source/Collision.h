#ifndef COLLISION_H
#define COLLISION_H



#include "Collidable.h"



class Collision {
public:
	Collision();
	
	float x, y;		//Point of contact
	Collidable * lhs;
	Collidable * rhs;
};



static Collision * Collides(Collidable *, Collidable *);



#endif