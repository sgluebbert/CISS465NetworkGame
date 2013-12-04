#include "Pickup_Factory.h"



Pickup_Factory::Pickup_Factory(float _x, float _y) {
	Set_Group(FACTORY_GROUP);

    team_id = NO_TEAM;

    dx = dy = force = torque = velocity = rotation = 0.0;

    x = bounding_volume.x = _x;
    y = bounding_volume.y = _y;
    draw_angle = angle = 0.0;
    draw_scale = bounding_volume.r = 8.0;
    drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
    drawing_box.w = 2 * bounding_volume.r;
    drawing_box.h = 2 * bounding_volume.r;

    pickup = NULL;
    reference = NULL;

    mass = _m;
    Set_Inertia(_r);

    spawn_timer.Set_Interval(15.0);
}

Pickup_Factory::~Pickup_Factory() {
	pickup = NULL;

	if (reference != NULL)
		delete reference;
}



void Pickup_Factory::Set_Pickup(Pickup * new_ref) {
	pickup = NULL;

	if (reference != NULL)
		delete reference;
	
	reference = new_ref;
	pickup = reference;
}

Pickup * Pickup_Factory::Grab_Pickup() {
	Pickup * temp = pickup;
	pickup = NULL;

	spawn_timer.Start();

	return temp;
}

void Pickup_Factory::Update(double dt) {
	spawn_timer.Update(dt);

	if (spawn_timer.Ended() && pickup == NULL)
		pickup = reference;
}

void Pickup_Factory::Draw() {
	if (pickup == NULL)
		return;

	//Draw Pickup's texture
}