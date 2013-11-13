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


<<<<<<< HEAD
	double rhs_normal;
	double rhs_force;

	bool DoCollide(Entity lhs, Entity rhs);
};
=======

static Collision * Collides(Collidable *, Collidable *);
>>>>>>> f880b59b79fb4ff98d4747192ddee7a233ae956f



#endif