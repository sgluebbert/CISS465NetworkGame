#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H



#include "Particle.h"
#include "../System.h"



static Particle * Create_Thrust_Particle() {
	Particle * thrust_particle = new Particle();

	return thrust_particle;
}

static Particle * Create_Smoke_Particle() {
	Particle * smoke_particle = new Particle();

    //Drawable Init
    smoke_particle->texture = surface_manager->smoke;
    smoke_particle->draw_angle = smoke_particle->angle = 0.0;
    smoke_particle->draw_scale = 16.0;
    smoke_particle->drawing_box.w = smoke_particle->drawing_box.h = 2 * smoke_particle->draw_scale;

    //RigidBody Init
    smoke_particle->velocity = 10.0;
    smoke_particle->dx = smoke_particle->dy = smoke_particle->force = smoke_particle->torque = smoke_particle->rotation = 0.0;
    smoke_particle->mass = 1.0;
	smoke_particle->Set_Inertia(smoke_particle->draw_scale);
    smoke_particle->x = smoke_particle->y = smoke_particle->drawing_box.x = smoke_particle->drawing_box.y = 0.0;

    //Motor Init
	smoke_particle->velocity_limit = smoke_particle->velocity;
	smoke_particle->force_motor = 200.0;
    smoke_particle->rotation_limit = smoke_particle->reverse_modifier = smoke_particle->torque_motor = 0.0;

    //Particle Init
	smoke_particle->age_timer.Set_Interval(2.0);

	return smoke_particle;
}

static Particle * Create_Explosion_Particle() {
	Particle * explosion_particle = new Particle();

	return explosion_particle;
}

static Particle * Create_Laser_Particle() {
	Particle * laser_particle = new Particle();

	return laser_particle;
}

static Particle * Create_Gauss_Particle() {
	Particle * gauss_particle = new Particle();

	return gauss_particle;
}

static Particle * Create_Rocket_Particle() {
	Particle * rocket_particle = new Particle();

	return rocket_particle;
}

static Particle * Create_Mine_Particle() {
	Particle * mine_particle = new Particle();

	return mine_particle;
}



#endif