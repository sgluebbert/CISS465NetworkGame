#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H



#include <SDL.h>

#include "Entity.h"



class Static_Entity : public Entity {
public:
    Static_Entity();
    ~Static_Entity();
    
    virtual void Update();
    virtual void Draw();
};



Static_Entity::Static_Entity() 
    : Entity() {}

Static_Entity::~Static_Entity() {
}

void Static_Entity::Draw() {
	Entity::Draw();
}

void Static_Entity::Update() {
    //Do Nothing, entity is static so it is event driven
}



#endif
