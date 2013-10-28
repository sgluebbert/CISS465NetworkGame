#ifndef ENTITY_H
#define ENTITY_H



#include <deque>
#include <SDL.h>

#include "AABB.h"
#include "BC.h"
#include "OBB.h"
#include "Rect.h"
#include "System.h"
#include "Timer.h"



class Entity {
public:
    Entity();
    ~Entity();

    void Set_Bounding_Box(BV *);
    void Set_Drawing_Box(Rect<double> &);
    void Set_Position(double, double);

    BV * Get_Bounding_Box();
    Rect<double> Get_Drawing_Box();

    //Will need to be passed more info to retain normal vectors and point of impact for collision resolution
    bool Is_Colliding(Entity *);

    void Map_To_Viewport(Rect<double>);
    void Map_To_World(Rect<double>);

    virtual void Serialize(Uint8 *, int &);
    virtual void Deserialize(Uint8 *, int &);
    
    virtual void Update(double);
    virtual void Draw();

    static std::deque<Entity *> entity_deque;
    
protected:
    BV * bounding_box;              //Hit Box for collisions
    Rect<double> drawing_box;       //Box Reference for drawing
    double x, y, angle;             //Center point and angle for rotation and drawing
    double mass;                    //For physics calculations
    bool update_flag;               //Update flag for Quad Tree updating and networking
};



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



void Entity::Serialize(Uint8 * arr, int & size) {
    //Add to the byte array
}

void Entity::Deserialize(Uint8 * arr, int & size) {
    //Remove from the byte array
}



void Entity::Update(double dt) {
    if (bounding_box != NULL)
        bounding_box->Update(dt, x, y);
}

void Entity::Draw() {
    //SDL_BlitSurface(surface_ptr, NULL, WINDOW, &bounding_box);
}



#endif
