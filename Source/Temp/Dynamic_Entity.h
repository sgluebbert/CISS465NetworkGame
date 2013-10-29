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



#endif
