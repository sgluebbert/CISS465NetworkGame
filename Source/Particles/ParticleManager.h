#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H



#include "Particle.h"
#include "../System.h"



static Particle * Create_Thrust_Particle() {
	Particle * thrust_particle = new Particle();

	return thrust_particle;
}

static Particle * Create_Smoke_Particle() {
	Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->smoke;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 16.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->velocity = 10.0;
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
	temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
	temp_particle->velocity_limit = temp_particle->velocity;
    temp_particle->rotation_limit = temp_particle->reverse_modifier = temp_particle->force_motor = temp_particle->torque_motor = 0.0;

    //Particle Init
	temp_particle->age_timer.Set_Interval(2.0);

	return temp_particle;
}

static Particle * Create_Shatter_Particle() {
	Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->shatter;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 4.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->velocity = 30.0;
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
	temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
	temp_particle->velocity_limit = temp_particle->velocity;
	temp_particle->torque_motor = 30.0;
    temp_particle->rotation_limit = 45.0;
    temp_particle->reverse_modifier = temp_particle->force_motor = 0.0;

    //Particle Init
	temp_particle->age_timer.Set_Interval(1.0);

	return temp_particle;
}

static Particle * Create_Explosion_Particle() {
	Particle * explosion_particle = new Particle();

	return explosion_particle;
}

static Particle * Create_Laser_Particle() {
	Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->bullet;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 8.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->velocity = 100.0;
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
	temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
	temp_particle->velocity_limit = temp_particle->velocity;
	temp_particle->force_motor = 200.0;
    temp_particle->rotation_limit = temp_particle->reverse_modifier = temp_particle->torque_motor = 0.0;

    //Particle Init
	temp_particle->age_timer.Set_Interval(2.0);

	return temp_particle;
}

static Particle * Create_Gauss_Particle() {
	Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->bullet;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 8.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->velocity = 200.0;
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
    temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
    temp_particle->velocity_limit = temp_particle->velocity;
    temp_particle->force_motor = 200.0;
    temp_particle->rotation_limit = temp_particle->reverse_modifier = temp_particle->torque_motor = 0.0;

    //Particle Init
    temp_particle->age_timer.Set_Interval(2.0);

	return temp_particle;
}

static Particle * Create_Rocket_Particle() {
    Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->bullet;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 8.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->velocity = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
    temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
    temp_particle->velocity_limit = 200.0;
    temp_particle->force_motor = 100.0;
    temp_particle->rotation_limit = temp_particle->reverse_modifier = temp_particle->torque_motor = 0.0;

    //Particle Init
    temp_particle->age_timer.Set_Interval(2.0);

    return temp_particle;
}

static Particle * Create_Mine_Particle() {
    Particle * temp_particle = new Particle();

    //Drawable Init
    temp_particle->texture = surface_manager->bullet;
    temp_particle->draw_angle = temp_particle->angle = 0.0;
    temp_particle->draw_scale = 8.0;
    temp_particle->drawing_box.w = temp_particle->drawing_box.h = 2 * temp_particle->draw_scale;

    //RigidBody Init
    temp_particle->dx = temp_particle->dy = temp_particle->force = temp_particle->torque = temp_particle->velocity = temp_particle->rotation = 0.0;
    temp_particle->mass = 1.0;
    temp_particle->Set_Inertia(temp_particle->draw_scale);
    temp_particle->x = temp_particle->y = temp_particle->drawing_box.x = temp_particle->drawing_box.y = 0.0;

    //Motor Init
    temp_particle->velocity_limit = temp_particle->rotation_limit = temp_particle->reverse_modifier = temp_particle->force_motor = temp_particle->torque_motor = 0.0;

    //Particle Init
    temp_particle->age_timer.Set_Interval(90.0);

    return temp_particle;
}



#endif