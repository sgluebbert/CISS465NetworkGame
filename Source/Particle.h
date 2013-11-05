#ifndef PARTICLE_H
#define PARTICLE_H



#include <vector>

#include "Clock.h"
#include "Entity.h"



class Particle : public Entity {
public:
	Particle();
	Particle(Particle &);
	~Particle();
    
    virtual void Update(double);
    virtual void Draw();

    static std::vector<Particle *> particles;
    //static Quad_Tree<Particle *> particle_tree;

    static void Initialize();
    static void Cleanup();

    static void Update_Particles();
    static void Draw_Particles();

	double age;
	double max_age;

	bool Is_Dead();
};



#endif