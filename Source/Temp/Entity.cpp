#include "Entity.h"
#include "System.h"
#include "Timer.h"



Entity::Entity() {
    angle = x = y = 0.0;

    bounding_box = NULL;
    update_flag = false;
}

Entity::~Entity() {
}


    
void Entity::Set_Bounding_Box(BV * _bounding_box) {
    bounding_box = _bounding_box;
}

void Entity::Set_Drawing_Box(Rect<double> & _drawing_box) {
    drawing_box = _drawing_box;
}

void Entity::Set_Position(double _x, double _y) {
    x = _x;
    y = _y;
}

BV * Entity::Get_Bounding_Box() {
    return bounding_box;
}

Rect<double> Entity::Get_Drawing_Box() {
    return drawing_box;
}

void Entity::Map_To_Viewport(Rect<double> viewport) {
    x -= viewport.x;
    y -= viewport.y;
}

void Entity::Map_To_World(Rect<double> viewport) {
    x += viewport.x;
    y += viewport.y;
}

bool Entity::Is_Colliding(Entity * other) {
    if (bounding_box == NULL)
        return false;

    return false;
}



void Entity::Serialize(Parser & parser) {
    //Write to the byte array
}

void Entity::Deserialize(Parser & parser) {
    //Read from the byte array
}



void Entity::Update(double dt) {
    if (bounding_box != NULL)
        bounding_box->Update(dt, x, y);
}

void Entity::Draw() {
    //SDL_BlitSurface(surface_ptr, NULL, WINDOW, &bounding_box);
}