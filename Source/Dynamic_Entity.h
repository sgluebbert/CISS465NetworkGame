#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H



#include "Entity.h"



class Dynamic_Entity : public Entity {
public:
    Dynamic_Entity();
    ~Dynamic_Entity();
    
    virtual void Update(double);
    virtual void Draw();
    
    void Set_Max_Velocity(double);
    void Set_Force(double);
    void Set_Turn_Rate(double);

    void Turn_Left(double);
    void Turn_Right(double);
    void Calculate_Speed(double);
    void Calculate_Velocity(double);
    void Move(double);

protected:
    double dx, dy;                      //Vector
    double velocity, max_velocity;      //units per second
    double reverse_modifier;            //Percent
    double force;                       //
    double inertia;                     //For torque calculations
    double turn_rate;                   //Radians per second
};



Dynamic_Entity::Dynamic_Entity()
    : Entity() {
    dx = dy = 0.0;
    velocity = max_velocity = 0.0;
    reverse_modifier = 0.0;
    force = 0.0;
    turn_rate = 0.0;
}

Dynamic_Entity::~Dynamic_Entity() {
}



void Dynamic_Entity::Set_Max_Velocity(double _max) {
    if (_max < 0)
        return;

    max_velocity = _max;
}

void Dynamic_Entity::Set_Force(double _force) {
    if (_force < 0)
        return;

    force = _force;
}

void Dynamic_Entity::Set_Turn_Rate(double _turn_rate) {
    if (_turn_rate < 0)
        return;

    turn_rate = _turn_rate;
}



void Dynamic_Entity::Turn_Left(double dt) {
    angle += turn_rate * dt;
    if (angle >= 360)
        angle = angle - 360;
}

void Dynamic_Entity::Turn_Right(double dt) {
    angle -= turn_rate * dt;
    if (angle < 0)
        angle = 360 + angle;
}

void Dynamic_Entity::Calculate_Speed(double dt) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][0];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][1];
}

void Dynamic_Entity::Calculate_Velocity(double dt) {
    if (velocity > max_velocity)
        velocity = max_velocity;
    if (velocity < (-max_velocity * reverse_modifier))
        velocity = (-max_velocity * reverse_modifier);
}

void Dynamic_Entity::Move(double dt) {
    x += dx * dt;
    y -= dy * dt;
}



void Dynamic_Entity::Draw() {
    Entity::Draw();
}

void Dynamic_Entity::Update(double dt) {
    Calculate_Velocity(dt);
    Calculate_Speed(dt);
    Move(dt);
}



#endif
