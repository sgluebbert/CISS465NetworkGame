#ifndef COLLISION_H
#define COLLISION_H



#include <utility>
#include <cmath>

#include "Collidable.h"
#include "Asteroid.h"
#include "Moon.h"
#include "Pickup_Factory.h"
#include "Particles/Particle.h"
#include "Planet.h"
#include "Ship.h"



class Collision {
public:
	virtual void ResolveCollision(Collidable *, Collidable *) = 0;
};


/*Ship Collision Callbacks*/
/////////////////////////////////////////////////////////
class ShipToShipCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class ShipToBulletCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class ShipToPlanetCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class ShipToMoonCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class ShipToAsteroidCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class ShipToFactoryCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};
/////////////////////////////////////////////////////////


/*Bullet Collision Callbacks*/
/////////////////////////////////////////////////////////
class BulletToPlanetCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class BulletToMoonCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};

class BulletToAsteroidCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};
/////////////////////////////////////////////////////////


/*Planet Collision Callbacks*/
/////////////////////////////////////////////////////////
class PlanetToAsteroidCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};
/////////////////////////////////////////////////////////


/*Moon Collision Callbacks*/
/////////////////////////////////////////////////////////
class MoonToAsteroidCollision : public Collision {
public:
	void ResolveCollision(Collidable *, Collidable *);
};
/////////////////////////////////////////////////////////

static void Collides(Collidable * lhs, Collidable * rhs) {
	if (lhs->mask[rhs->group] == false)
		return;
	if (rhs->mask[lhs->group] == false)
		return;

	float dx = lhs->bounding_volume.x - rhs->bounding_volume.x;
	float dy = lhs->bounding_volume.y - rhs->bounding_volume.y;
	float dr = lhs->bounding_volume.r + rhs->bounding_volume.r;

	if (dx * dx + dy * dy >= dr * dr)
		return;

	/*dx = std::abs(dx);
	dy = std::abs(dy);

	float a = lhs->bounding_volume.x * rhs->bounding_volume.r + rhs->bounding_volume.x * lhs->bounding_volume.r;
	float b = lhs->bounding_volume.y * rhs->bounding_volume.r + rhs->bounding_volume.y * lhs->bounding_volume.r;

	temp->x = a / dr;
	temp->y = b / dr;*/

	return;
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