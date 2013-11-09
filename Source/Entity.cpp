#include "Entity.h"
#include "System.h"
#include "Timer.h"



Entity::Entity() {
    angle = x = y = w = h = 0.0;
    dx = dy = 0.0;
    velocity = max_velocity = 0.0;
    reverse_modifier = 0.0;
    mass = force = inertia = 0.0;
    turn_rate = 0.0;
}

Entity::~Entity() {
}



void Entity::Map_To_Viewport(Rect<double> viewport) {
    x -= viewport.x;
    y -= viewport.y;
}

void Entity::Map_To_World(Rect<double> viewport) {
    x += viewport.x;
    y += viewport.y;
}


NetString *Entity::Serialize() {
    NetString * string = new NetString();

    if (!string->WriteFloat(x) && !string->WriteFloat(y) && !string->WriteFloat(angle)) {
        delete string;
        return NULL;
    }

    return string;
}

bool Entity::Deserialize(NetString *string) {
    if (!string->ReadFloat(x) && !string->ReadFloat(y) && !string->ReadFloat(angle))
        return false;
    return true;
}



void Entity::Turn_Left(double dt) {
    angle += turn_rate * dt;
    if (angle >= 360)
        angle = angle - 360;
}

void Entity::Turn_Right(double dt) {
    angle -= turn_rate * dt;
    if (angle < 0)
        angle = 360 + angle;
}

void Entity::Calculate_Velocity(double dt) {
    velocity += (force / mass) * dt;

    if (velocity > max_velocity)
        velocity = max_velocity;
    else if (velocity < (-max_velocity * reverse_modifier))
             velocity = (-max_velocity * reverse_modifier);
}

void Entity::Calculate_Speed(double dt) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][0];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][1];
}

void Entity::Move(double dt) {
    x += dx;
    y -= dy;
}



void Entity::Update(double dt) {
    Calculate_Velocity(dt);
    Calculate_Speed(dt);
    Move(dt);

    bounding_volume.Update(dt, x, y);
}

void Entity::Draw() {
}