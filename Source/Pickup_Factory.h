#ifndef PICKUP_FACTORY_H
#define PICKUP_FACTORY_H



#include "Collidable.h"
#include "Drawing/Drawable.h"
#include "Pickup.h"
#include "RigidBody.h"
#include "Timer.h"



class Pickup_Factory : public Collidable, public Drawable, public Rigid_Body {
public:
	Pickup_Factory(float, float);
	~Pickup_Factory();

	void Set_Pickup(Pickup *);
	Pickup * Grab_Pickup();

	void Update(double);
	void Draw();

private:
	Timer spawn_timer;
	Pickup * pickup;
	Pickup * reference;
};



#endif