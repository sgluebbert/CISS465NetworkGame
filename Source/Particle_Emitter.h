#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H



#include <iostream>

#include "Clock.h"
#include "Particle.h"
#include "System.h"



class Particle_Emitter {
public:
	Particle_Emitter();
	~Particle_Emitter();
    
    virtual void Update();
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



Particle_Emitter::Particle_Emitter() {
	active = false;
	max_particle_count = particle_count = spawn_count = 0;
    spawn_delay = spawn_timer = 0.0;

    max_particle_count_variance = spawn_count_variance = 0;
    max_age_variance = velocity_variance = angle_variance = throttle_variance = spawn_timer_variance = 0;

    reference = NULL;
}

Particle_Emitter::~Particle_Emitter() {
	delete reference;
}



void Particle_Emitter::Update() {
    if (!active)
    	return;

    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    int count_var = rand() % spawn_count_variance;

	if (particle_count >= max_particle_count && max_particle_count > 0) 
		Deactivate();
	else if (spawn_timer >= spawn_delay) {
    	int count_var = rand() % spawn_count_variance;

		for (int i = 0; i < spawn_count + count_var; i++) {
			Particle * temp = new Particle(*reference);
			temp->Adjust_Age(-max_age_variance + (float)rand() / ((float)RAND_MAX / (2 * max_age_variance)));
			temp->Adjust_Angle(-angle_variance + (float)rand() / ((float)RAND_MAX / (2 * angle_variance)));
			temp->Adjust_Throttle(-throttle_variance + (float)rand() / ((float)RAND_MAX / (2 * throttle_variance)));
			temp->Adjust_Velocity(-velocity_variance + (float)rand() / ((float)RAND_MAX / (2 * velocity_variance)));
			Particle::particles.push_back(temp);
		}
		
		particle_count += (spawn_count + count_var);
		spawn_timer = 0.0;
	}
	else
    	spawn_timer += dt;
}

void Particle_Emitter::Draw() {
	if (reference == NULL)
		return;
}



void Particle_Emitter::Activate() {
	if (reference == NULL)
		return;

	active = true;
	spawn_timer = spawn_delay - spawn_timer_variance + (float)rand() / ((float)RAND_MAX / (2 * spawn_timer_variance));
	particle_count = 0;
}

void Particle_Emitter::Deactivate() {
	active = false;
}



void Particle_Emitter::Set_Particle(Particle * new_particle) {
	reference = new_particle;
}

void Particle_Emitter::Set_Particle_Count(int new_max) {
	max_particle_count = new_max;
}

void Particle_Emitter::Set_Spawn_Count(int new_count) {
	spawn_count = new_count;
}

void Particle_Emitter::Set_Spawn_Delay(double new_delay) {
	spawn_delay = new_delay;
}

void Particle_Emitter::Set_Max_Age(double new_age) {
	if (reference == NULL)
		return;

	reference->Set_Age(new_age);
}

void Particle_Emitter::Set_Starting_Angle(double new_angle) {
	if (reference == NULL)
		return;
	
	reference->Set_Angle(new_angle);
}

void Particle_Emitter::Set_Starting_Throttle(double throttle) {
	if (reference == NULL)
		return;
	
	reference->Set_Throttle(throttle);
}

void Particle_Emitter::Set_Starting_Velocity(double new_velocity) {
	if (reference == NULL)
		return;
	
	reference->Set_Velocity(new_velocity);
}



void Particle_Emitter::Set_Particle_Count_Variance(int variance) {
	max_particle_count_variance = variance;
}

void Particle_Emitter::Set_Spawn_Count_Variance(int variance) {
	spawn_count_variance = variance;
}

void Particle_Emitter::Set_Spawn_Delay_Variance(double variance) {
	spawn_timer_variance = variance;
}

void Particle_Emitter::Set_Max_Age_Variance(double variance) {
	max_age_variance = variance;
}

void Particle_Emitter::Set_Starting_Angle_Variance(double variance) {
	angle_variance = variance;
}

void Particle_Emitter::Set_Starting_Throttle_Variance(double variance) {
	throttle_variance = variance;
}

void Particle_Emitter::Set_Starting_Velocity_Variance(double variance) {
	velocity_variance = variance;
}



bool Particle_Emitter::Is_Active() {
	return active;
}



#endif