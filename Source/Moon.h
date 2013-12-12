#ifndef MOON_H
#define MOON_H



#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "Collidable.h"
#include "Ship.h"



enum MOON_TYPE { HEALTH, SHIELD, POWER, ARMOR, DEAD_MOON};



class Moon : public Entity, public Collidable
{
public:

	Moon(float _offset, float _angle, float _ocx, float _ocy, float _m, float _r, float fr);
	~Moon();

	virtual void Update(double);
    virtual void Draw();

    void TakeDamage(float);
    void DrawGravityField();
    void DistributeResource();
    void RemoveResource(Team);
    void DistributeRegen(double);

    Texture * moon;
    Texture * field;

    float orbit_speed;
    float orbit_center_x, orbit_center_y;
    float orbit_radius;
    float health, max_health;
    float alignment;
    float capture_rate;
    float boost_factor;
    float field_radius;
    bool alive, just_captured;

    MOON_TYPE type;
};



#endif