#include "Entity.h"
#include "System.h"



Entity::Entity() {
    angle = x = y = w = h = 0.0;
    dx = dy = 0.0;
    velocity = max_velocity = 0.0;
    reverse_modifier = 0.0;
    mass = force = inertia = 0.0;
    turn_rate = 0.0;
}

Entity::Entity(float _x, float _y, float _w, float _h, float _angle, float _mass, float _velocity, float _r)
    : x(_x), y(_y), w(_w), h(_h), angle(_angle), mass(_mass), velocity(_velocity)
{
    bounding_volume.x = _x;
    bounding_volume.y = _y;
    bounding_volume.r = _r;
}


Entity::~Entity() {
}



NetString *Entity::Serialize() {
    NetString * string = new NetString();

    if (!string->WriteFloat(x) || !string->WriteFloat(y) || !string->WriteFloat(angle)) {
        delete string;
        return NULL;
    }

    return string;
}

bool Entity::Deserialize(NetString *string) {
    if (!string->ReadFloat(x) || !string->ReadFloat(y) || !string->ReadFloat(angle))
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
    velocity *= 0.95;//FRICTION_COEFFICENT;
    velocity += (force / mass) * dt;

    if (velocity > max_velocity)
        velocity = max_velocity;
    else if (velocity < (-max_velocity * reverse_modifier))
             velocity = (-max_velocity * reverse_modifier);
}

void Entity::Calculate_Speed(double dt) {
    //TRIG_TABLE isn't working correctly, for some reason it is always 0 within this function, and elsewhere is correct
    dx = velocity * cos(angle * PI / 180) * dt;//TRIG_TABLE[int(angle / 5.0)][1] * dt;
    dy = velocity * sin(angle * PI / 180) * dt;//TRIG_TABLE[int(angle / 5.0)][0] * dt;
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