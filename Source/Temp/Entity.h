#ifndef ENTITY_H
#define ENTITY_H



#include <deque>
#include <SDL.h>

#include "AABB.h"
#include "BC.h"
#include "OBB.h"
#include "Rect.h"



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



#endif
