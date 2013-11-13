#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H



#include "Particle.h"
#include "../TextureManager.h"



class Particle_Manager {
public:
	static Particle_Manager * Get_Instance();
	static void Delete_Instance();

	Particle * thrust_particle;
	Particle * smoke_particle;
	Particle * explosion_particle;
	Particle * laser_particle;
	Particle * gauss_particle;
	Particle * rocket_particle;
	Particle * mine_particle;
	
private:
    Particle_Manager();
    ~Particle_Manager();

	static Particle_Manager * instance;
};



#endif