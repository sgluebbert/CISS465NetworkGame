#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H



#include <iostream>

#include "Particle.h"



class Particle_Emitter {
public:
	Particle_Emitter();
	~Particle_Emitter();
    
    virtual void Update(double, float, float);
    virtual void Draw();

    //Activation Methods
	void Activate();
	void Deactivate();

	//Particle Reference
	void Set_Particle(Particle *);

	//Configuration Methods
	void Set_Particle_Count(int);
	void Set_Spawn_Count(int);
	void Set_Spawn_Delay(double);
	void Set_Max_Age(double);
	void Set_Starting_Angle(double);
	void Set_Starting_Throttle(double);
	void Set_Starting_Velocity(double);

	//Variance Configuration Methods
	void Set_Particle_Count_Variance(int);
	void Set_Spawn_Count_Variance(int);
	void Set_Spawn_Delay_Variance(double);
	void Set_Max_Age_Variance(double);
	void Set_Starting_Angle_Variance(double);
	void Set_Starting_Throttle_Variance(double);
	void Set_Starting_Velocity_Variance(double);

	bool Is_Active();

private:
    /*
    active 				| whether the emitter is active or not
    particle_count 		| count of particles created since switched active
    max_particle_count 	| number of particles to create before deactivating
    spawn_count 		| number of particles to create each frame
    spawn_delay 		| time to wait before spawning a new set of particles
    spawn_timer 		| time currently waited to spawn a new set
    reference 			| Particle used as a base for all particles spawned, never gets used other than for copy constructor
    */

	bool active;
	int particle_count, max_particle_count;
	int spawn_count;
    double spawn_delay, spawn_timer;

	int max_particle_count_variance;
	int spawn_count_variance;
    double max_age_variance;
    double velocity_variance;
    double angle_variance;
    double throttle_variance;
    double spawn_timer_variance;

    Particle * reference;
};



#endif