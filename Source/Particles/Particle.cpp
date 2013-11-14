#include "Particle.h"



Particle::Particle() {
    //Drawable Init
    draw_scale = draw_angle = 0.0;

    //RigidBody Init
    dx = dy = force = torque = velocity = rotation = 0.0;
    mass = inertia = 1.0;
    x = y = angle = 0.0;

    //Motor Init
    velocity_limit = rotation_limit = reverse_modifier = force_motor = torque_motor = 0.0;
}

Particle::Particle(Particle * p) {
    //Drawable Init
    drawing_box = p->drawing_box;
    draw_scale = p->draw_scale;
    draw_angle = p->draw_angle;

    //RigidBody Init
    dx = dy = force = torque = velocity = rotation = 0.0;

    mass = p->mass;
    inertia = p->inertia;

    x = p->x;
    y = p->y;
    angle = p->angle;

    //Motor Init
    velocity_limit = p->velocity_limit;
    rotation_limit = p->rotation_limit;

    reverse_modifier = p->reverse_modifier;

    force_motor = p->force_motor;
    torque_motor = p->torque_motor;

    //Particle Init
	age_timer.Set_Interval(p->age_timer.Get_Interval());
    age_timer.Start();

    particles.push_back(this);
}

Particle::~Particle() {
}

bool Particle::Is_Dead() {
	return (age_timer.Ended());
}



void Particle::Limit_Motor() {
    if (rotation > rotation_limit)
        rotation = rotation_limit;
    
    if (velocity > velocity_limit)
        velocity = velocity_limit;

    if (velocity < velocity_limit * reverse_modifier)
        velocity = velocity_limit * reverse_modifier;
}

void Particle::Update(double dt) {
    Apply_Force(force_motor, draw_angle, 0, 0);
    Apply_Torque(torque_motor);

    Calculate_Velocity(dt);
    Calculate_Rotation(dt);

    Limit_Motor();

    Calculate_Vector(dt);
    Move(dt);

    drawing_box.Update(dx, dy);
    age_timer.Update(dt);
}

void Particle::Draw() {
    Entity::Draw();
}



std::deque<Particle *> Particle::particles;

void Particle::Update_Particles(double dt) {
    for (int i = 0; i < particles.size(); i++)
        particles[i]->Update(dt);
}

void Particle::Draw_Particles() {
    for (int i = 0; i < particles.size(); i++)
        particles[i]->Draw();
}