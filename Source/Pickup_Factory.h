#ifndef PICKUP_FACTORY_H
#define PICKUP_FACTORY_H



#include "Collidable.h"
#include "Drawing/Drawable.h"
#include "Pickup.h"
#include "RigidBody.h"
#include "Timer.h"



class Pickup_Factory : public Collidable, public Drawable, public Rigid_Body {
public:
	Pickup_Factory();
	~Pickup_Factory();

	Timer spawn_delay;
	Pickup * pickup;
};



#endif