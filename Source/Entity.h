#ifndef ENTITY_H
#define ENTITY_H



#include <deque>

#include "Circle.h"
#include "Rect.h"



class Entity {
public:
    Entity();
    ~Entity();

    void Map_To_Viewport(Rect<double>);
    void Map_To_World(Rect<double>);

    virtual void Serialize();
    virtual void Deserialize();
    
    void Turn_Left(double);
    void Turn_Right(double);
    void Calculate_Speed(double);
    void Calculate_Velocity(double);
    void Move(double);
    
    virtual void Update(double);
    virtual void Draw();
    
    Circle<double> bounding_volume;     //Hit Box for collisions
    Rect<double> drawing_box;           //Box Reference for drawing?

    double x, y, w, h, angle;           //Center point and angle for rotation and drawing
    double dx, dy;                      //Vector
    double mass;                        //For physics calculations
    double velocity, max_velocity;      //units per second
    double reverse_modifier;            //Percent
    double force;                       //
    double inertia;                     //For torque calculations
    double turn_rate;                   //Radians per second
};



#endif
