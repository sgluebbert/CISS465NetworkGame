#ifndef MOON_H
#define MOON_H



#include "Drawing/Draw.h"
#include "Drawing/Texture.h"
#include "Entity.h"
#include "Collidable.h"
#include "Ship.h"



enum MOON_TYPE { HEALTH, SHIELD, POWER, ARMOR };



class Moon : public Entity, public Collidable
{
public:

	Moon(Team t, float _x, float _y, float _m, float _r, float fr);
	~Moon();

	virtual void Update(double);
    virtual void Draw();

    void Move();
    void TakeDamage();
    void DrawGravityField();
    void DistributeResource(double);

    Texture * moon;
    Texture * field;

    float health, max_health;
    float field_radius;
    float boost_factor;
    bool alive;

    MOON_TYPE type;
};



#endif