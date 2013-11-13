#include "ParticleManager.h"



Particle_Manager::Particle_Manager() {
	/*Thrust Particle*/
	thrust_particle = new Particle();

	thrust_particle->w = 20.0;
	thrust_particle->h = 20.0;

	thrust_particle->mass = 1.0;

	thrust_particle->max_age = 1.0;

	/*Smoke Particle*/
	smoke_particle = new Particle();

	smoke_particle->w = 20.0;
	smoke_particle->h = 20.0;

	smoke_particle->velocity = 100.0;
	smoke_particle->max_velocity = 100.0;
	
	smoke_particle->force = 0.0;
	smoke_particle->mass = 1.0;

	smoke_particle->max_age = 2.0;

	/*Explosion Particle*/
	explosion_particle = new Particle();

	explosion_particle->w = 20.0;
	explosion_particle->h = 20.0;

	explosion_particle->mass = 1.0;

	explosion_particle->max_age = 5.0;

	laser_particle = new Particle();
	gauss_particle = new Particle();
	rocket_particle = new Particle();
	mine_particle = new Particle();
}

Particle_Manager::~Particle_Manager() {
	delete thrust_particle;
	delete smoke_particle;
	delete explosion_particle;
	delete laser_particle;
	delete gauss_particle;
	delete rocket_particle;
	delete mine_particle;
}



Particle_Manager * Particle_Manager::Get_Instance() {
	if (instance == NULL)
		instance = new Sound_Manager();
		
	return instance;
}

void Particle_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}