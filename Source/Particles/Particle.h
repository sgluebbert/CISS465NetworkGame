#ifndef PARTICLE_H
#define PARTICLE_H



#include <deque>

#include "../Collidable.h"
#include "../Entity.h"
#include "../Motor.h"
#include "../Timer.h"



class Particle : public Entity, public Motor, public Collidable {
public:
	Particle();
	Particle(Particle *, float &, float &);
	~Particle();
    
    virtual void Limit_Motor();
    virtual void Update(double);
    virtual void Draw();

	virtual bool Is_Dead();

	Timer age_timer;
    float distance_travelled, range;
    Team team_id;
    float weapon_damage;

    //Need to turn this into a priority queue
    static std::deque<Particle *> particles;
};



#endif