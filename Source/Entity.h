#ifndef ENTITY_H
#define ENTITY_H



#include "Drawing/Draw.h"
#include "Drawing/Drawable.h"
#include "RigidBody.h"



enum Team {	NO_TEAM, RED_TEAM, BLUE_TEAM, NEUTRAL_TEAM	};



class Entity : public Drawable, public Rigid_Body {
public:
    Entity();
    
    virtual void Update(double);
    virtual void Draw();
    virtual void Sync();

    Team team_id;
};



#endif
