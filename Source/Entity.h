#ifndef ENTITY_H
#define ENTITY_H



#include "Networking/NetString.h"

#include "Circle.h"
#include "Rect.h"



class Entity {
public:
    Entity();
    ~Entity();

    virtual NetString * Serialize();
    virtual bool Deserialize(NetString *);
    
    void Turn_Left(double);
    void Turn_Right(double);
    void Calculate_Speed(double);
    void Calculate_Velocity(double);
    void Move(double);
    
    virtual void Update(double);
    virtual void Draw();
    
    Circle<double> bounding_volume;     //Hit Box for collisions

    float x, y, w, h, angle;           //Center point and angle for rotation and drawing
    float dx, dy;                      //Vector
    float mass;                        //For physics calculations
    float velocity, max_velocity;      //units per second
    float reverse_modifier;            //Percent
    float force;                       //
    float inertia;                     //For torque calculations
    float turn_rate;                   //Degrees per second

    //static Quad_Tree drawable;
    //static Quad_Tree collidable;

    //static void Update_Trees();
};



#endif
