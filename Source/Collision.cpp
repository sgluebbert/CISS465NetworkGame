#include "Collision.h"



Collision::Collision() {
	x = y = 0.0;
	lhs = rhs = NULL;
}



static Collision * Collides(Collidable * lhs, Collidable * rhs) {
	Collision * temp = new Collision();

	float dx = lhs->bounding_volume.x - rhs->bounding_volume.x;
	float dy = lhs->bounding_volume.y - rhs->bounding_volume.y;
	float dr = lhs->bounding_volume.r + rhs->bounding_volume.r;

	if (dx * dx + dy * dy >= dr * dr) //If they don't collide, return NULL pointer
		return NULL;

	temp->lhs = lhs;
	temp->rhs = rhs;

	return temp;
}

bool Collision::DoCollide(Entity lhs, Entity rhs)
{
	// double dx = lhs.bounding_volume.x - rhs.bounding_volume.x;
	// double dy = lhs.bounding_volume.y - rhs.bounding_volume.y;
	// double dr = lhs.bounding_volume.r + rhs.bounding_volume.r;

	// if (dx * dx + dy * dy >= dr * dr)
	// { 
	// 	return 0;
	// }

	return 1;
}