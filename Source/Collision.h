#ifndef COLLISION_H
#define COLLISION_H



#include <utility>
#include <cmath>

#include "Collidable.h"
// #include "Asteroid.h"
// //#include "Bullet.h"
// #include "Capture_Zone.h"
// #include "Moon.h"
// #include "Powerup.h"
// #include "Particle.h"
// #include "Planet.h"
// #include "Ship.h"



class Collision {
public:
	std::pair<Collision_Group, Collision_Group> Get_Pair();

	float x, y;
	Collidable * lhs;
	Collidable * rhs;
};



class Collision_Event {
public:
	//void ResolveCollision(Collidable *, Collidable *, float, float) = 0;
};



static Collision * Collides(Collidable * lhs, Collidable * rhs) {
	Collision * temp = new Collision();
	if (lhs->mask[rhs->group] == false)
		return NULL;
	if (rhs->mask[lhs->group] == false)
		return NULL;

	float dx = lhs->bounding_volume.x - rhs->bounding_volume.x;
	float dy = lhs->bounding_volume.y - rhs->bounding_volume.y;
	float dr = lhs->bounding_volume.r + rhs->bounding_volume.r;

	if (dx * dx + dy * dy >= dr * dr)
		return NULL;

	temp->lhs = lhs;
	temp->rhs = rhs;

	dx = std::abs(dx);
	dy = std::abs(dy);

	float a = lhs->bounding_volume.x * rhs->bounding_volume.r + rhs->bounding_volume.x * lhs->bounding_volume.r;
	float b = lhs->bounding_volume.y * rhs->bounding_volume.r + rhs->bounding_volume.y * lhs->bounding_volume.r;

	temp->x = a / dr;
	temp->y = b / dr;

	return temp;
}

static bool DoCollide(Collidable * lhs, Collidable * rhs) {
	float dx = lhs->bounding_volume.x - rhs->bounding_volume.x;
	float dy = lhs->bounding_volume.y - rhs->bounding_volume.y;
	float dr = lhs->bounding_volume.r + rhs->bounding_volume.r;

	if (dx * dx + dy * dy < dr * dr)
		return true;

	return false;
}



#endif