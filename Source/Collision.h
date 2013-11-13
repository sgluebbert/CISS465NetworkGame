#ifndef COLLISION_H
#define COLLISION_H



#include "Entity.h"



class Collision {
public:
	Collision();
	
	double x, y;		//Point of contact

	double lhs_normal;
	double lhs_force;

	double rhs_normal;
	double rhs_force;

	bool DoCollide(Entity lhs, Entity rhs);
};



#endif