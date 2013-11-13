#ifndef ENTITY_H
#define ENTITY_H



#include "Drawing/Draw.h"
#include "Drawing/Drawable.h"
#include "RigidBody.h"



class Entity : public Drawable, public Rigid_Body {
public:
    Entity();

    virtual void Update(double);
    virtual void Draw();
};



#endif
