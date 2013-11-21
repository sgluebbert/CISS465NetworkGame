#include "Particle.h"



std::deque<Particle *> Particle::particles;



Particle::Particle() {
    //Drawable Init
    drawing_box.x = drawing_box.y = drawing_box.w = drawing_box.h = 0.0;
    draw_scale = draw_angle = 0.0;

    //RigidBody Init
    dx = dy = force = torque = velocity = rotation = 0.0;
    mass = inertia = 1.0;
    x = y = angle = 0.0;
    
    //Motor Init
    velocity_limit = rotation_limit = reverse_modifier = force_motor = torque_motor = 0.0;
}

Particle::Particle(Particle * p, float & _x, float & _y) {
    Particle::particles.push_back(this);
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    Set_Group(NO_GROUP);

    //Drawable Init
    texture = p->texture;
    drawing_box.x = _x - p->draw_scale / 2.0;
    drawing_box.y = _y - p->draw_scale / 2.0;
    drawing_box.w = drawing_box.h = draw_scale = p->draw_scale;
    draw_angle = p->draw_angle;

    //RigidBody Init
    dx = dy = force = torque = 0.0;

    velocity = p->velocity;
    rotation = p->rotation;
    mass = p->mass;
    inertia = p->inertia;

    x = _x;
    y = _y;
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
    distance_travelled = 0;
    range = p->range;
}

Particle::~Particle() {
}

bool Particle::Is_Dead() {
    if (age_timer.Ended())
        return true;
    if (distance_travelled >= range)
        return true;

    return false;
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
    velocity *= FRICTION_COEFFICIENT;
    rotation *= FRICTION_COEFFICIENT;
        
    Apply_Force(force_motor, draw_angle, 0, 0);
    Apply_Torque(torque_motor);

    Calculate_Velocity(dt);
    Calculate_Rotation(dt);

    Limit_Motor();

    Calculate_Vector(dt);
    Move(dt);

    draw_angle = angle;
    drawing_box.Update(dx, -dy);
    age_timer.Update(dt);
    distance_travelled += velocity * dt;
}

void Particle::Draw() {
    Entity::Draw();
}