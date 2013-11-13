#ifndef ENTITY_H
#define ENTITY_H



#include "Drawing/Draw.h"
#include "Drawing/Drawable.h"
#include "RigidBody.h"



class Entity : public Drawable, public Rigid_Body {
public:
    Entity();
<<<<<<< HEAD
    Entity(float _x, float _y, float _w, float _h, float _angle, float _mass, float _velocity, float _r);
    ~Entity();

    virtual NetString * Serialize();
    virtual bool Deserialize(NetString *);
    
    void Turn_Left(double);
    void Turn_Right(double);
    void Calculate_Speed(double);
    void Calculate_Velocity(double);
    void Move(double);
    
=======

>>>>>>> f880b59b79fb4ff98d4747192ddee7a233ae956f
    virtual void Update(double);
    virtual void Draw();
};



#endif
