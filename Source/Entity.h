#ifndef ENTITY_H
#define ENTITY_H



#include "Networking/NetString.h"

#include "Circle.h"
#include "Rect.h"



class Entity {
public:
    Entity();
    ~Entity();

    void Map_To_Viewport(Rect<double>);
    void Map_To_World(Rect<double>);

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
    double dx, dy;                      //Vector
    double mass;                        //For physics calculations
    double velocity, max_velocity;      //units per second
    double reverse_modifier;            //Percent
    double force;                       //
    double inertia;                     //For torque calculations
    double turn_rate;                   //Degrees per second

    //static Quad_Tree drawable;
    //static Quad_Tree collidable;

    //static void Update_Trees();
};



#endif
