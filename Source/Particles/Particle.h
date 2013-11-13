#ifndef PARTICLE_H
#define PARTICLE_H



#include <deque>

#include "../Entity.h"
#include "../Motor.h"
#include "../Timer.h"



class Particle : public Entity, public Motor {
public:
	Particle();
	Particle(Particle *);
	~Particle();
    
    virtual void Limit_Motor();
    virtual void Update(double);
    virtual void Draw();

	Timer age_timer;
    Color color;

	bool Is_Dead();

    static std::deque<Particle *> particles;

    static void Update_Particles(double);
    static void Draw_Particles();
};



#endif