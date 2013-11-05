#include <cstdlib>

#include "Collision.h"



Collision::Collision() {
	x = y = 0.0;
	lhs_normal = rhs_normal = lhs_force = rhs_force = 0.0;
}

static Collision * Collides(Entity * lhs, Entity * rhs) {
	Collision * temp = new Collision();

	double dx = lhs->bounding_volume.x - rhs->bounding_volume.x;
	double dy = lhs->bounding_volume.y - rhs->bounding_volume.y;
	double dr = lhs->bounding_volume.r + rhs->bounding_volume.r;

	if (dx * dx + dy * dy >= dr * dr) //If they don't collide, return NULL pointer
		return NULL;

	temp->lhs_force = lhs->force;
	temp->rhs_force = rhs->force;

	return temp;
}