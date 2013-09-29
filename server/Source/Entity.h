#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Bullet.h"
#include "System.h"
#include <ctime>

class Entity {
public:
    Entity(Uint64, Uint32, int);
    
    void TurnLeft(double);
    void TurnRight(double);
    void CalculateSpeed(double);
    void CalculateVelocity(double);
    void Move(double);
    void TryFire();
    
    //virtual void Events(SDL_Event *);
    virtual void Update();
    
//protected:
    Uint64 ip;
    Uint32 port;
    clock_t last_input;
    int id;
    double x, y;
    double dx, dy;
    double width, height;
    
    double turn_rate;
    double angle;
    
    double throttle;
    double max_velocity;
    double velocity;
    double acceleration;
    double deceleration;
    bool move_forward, turn_left, turn_right, shoot;
};

Entity::Entity(Uint64 _ip, Uint32 _port, int _id)
    : ip(_ip), port(_port), id(_id) {
    x = 0.0;
    y = 0.0;
    dx = 0.0;
    dy = 0.0;
    width = 0.0;
    height = 0.0;
    angle = 0.0;
    turn_rate = 0.0;
    throttle = 0.0;
    velocity = 0.0;
    acceleration = 0.0;
    deceleration = 0.0;
    move_forward = turn_left = turn_right = shoot = false;
}

void Entity::CalculateSpeed(double delta) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][1];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][0];
}

void Entity::CalculateVelocity(double delta) {
    double tempA = acceleration * delta;
    double tempD = deceleration * delta;
    
    if (velocity > max_velocity * throttle) {
        velocity -= tempD;
        
        if (velocity < max_velocity * throttle)
            velocity = max_velocity * throttle;
    }
    else if (velocity < max_velocity * throttle) {
        velocity += tempA;
        
        if (velocity > max_velocity * throttle)
            velocity = max_velocity * throttle;
    }
}

void Entity::TurnLeft(double delta) {
    angle += turn_rate * delta;
    if (angle >= 360) angle = angle - 360;
}

void Entity::TurnRight(double delta) {
    angle -= turn_rate * delta;
    if (angle < 0) angle = 360 + angle;
}

void Entity::Move(double delta) {
    x += dx * delta;
    y -= dy * delta;
}

void Entity::Update() {
    double delta = 1.0 / 30.0;
    
    if (move_forward)
        throttle = 1;
    else
        throttle = 0;

    if (turn_left)
        TurnLeft(delta);
    if (turn_right)
        TurnRight(delta);
    if (shoot)
        TryFire();

    CalculateVelocity(delta);
    CalculateSpeed(delta);
    Move(delta);
}

void Entity::TryFire()
{
    // Bullet_List *bullet_list = Bullet_List::getInstance();

    // bullet_list->AddBullet(x + width / 2, y + height / 2, velocity + 60, angle);
}

#endif
