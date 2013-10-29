#ifndef PARTICLE_H
#define PARTICLE_H



#include <vector>

#include "Dynamic_Entity.h"



class Particle : public Dynamic_Entity {
public:
	Particle();
	Particle(Particle &);
	~Particle();

	void Set_Age(double);
	void Set_Angle(double);
	void Set_Throttle(double);
	void Set_Velocity(double);

	void Adjust_Age(double);
	void Adjust_Angle(double);
	void Adjust_Throttle(double);
	void Adjust_Velocity(double);
    
    virtual void Update(double);
    virtual void Draw();

    static std::vector<Particle *> particles;
    //static Quad_Tree<Particle *> particle_tree;

    static void Initialize();
    static void Cleanup();

    static void Update_Particles();
    static void Draw_Particles();

private:
	double age;
	double max_age;

	bool Is_Dead();
};



std::vector<Particle *> Particle::particles;
//Quad_Tree<Particle *> Particle::particle_tree;



Particle::Particle() {
	age = 0;
	max_age = 0;
}

Particle::Particle(Particle & p) {
    dx = dy = 0.0;
	age = 0;

    bounding_box = p.bounding_box;
    x = p.x;
    y = p.y;
    angle = p.angle;

    velocity = p.velocity;
    max_velocity = p.max_velocity;
    reverse_modifier = p.reverse_modifier;
    force = p.force;
    turn_rate = p.turn_rate;

	max_age = p.max_age;
}

Particle::~Particle() {
}

void Particle::Set_Age(double new_age) {
	max_age = new_age;
}

void Particle::Set_Angle(double new_angle) {
	angle = new_angle;
}

void Particle::Set_Throttle(double throttle) {
	if (throttle > 1.0)
		throttle = 1.0;
	if (throttle < reverse_modifier)
		throttle = reverse_modifier;
	
	velocity = max_velocity * throttle;
}

void Particle::Set_Velocity(double new_velocity) {
	velocity = new_velocity;
}

void Particle::Adjust_Age(double variance) {
	max_age += variance;
}

void Particle::Adjust_Angle(double variance) {
	angle += variance;
}

void Particle::Adjust_Throttle(double variance) {
	double throttle = velocity / max_velocity + variance;

	if (throttle > 1.0)
		throttle = 1.0;
	if (throttle < reverse_modifier)
		throttle = reverse_modifier;
	
	velocity = max_velocity * throttle;
}

void Particle::Adjust_Velocity(double variance) {
	velocity += variance;
}

bool Particle::Is_Dead() {
	return (age >= max_age);
}



void Particle::Update(double dt) {
    age += dt;

	Dynamic_Entity::Update(dt);
}

void Particle::Draw() {
    Entity::Draw();
}



void Particle::Initialize() {
	//Is there anything that really needs to go in here?
}

void Particle::Cleanup() {
	Particle::particles.clear();
	//Particle::particle_tree.Clear();
}

void Particle::Update_Particles() {
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

	for (int i = particles.size() - 1; i > -1; i--) {
		Particle::particles[i]->Update(dt);
		if (particles[i]->Is_Dead())
			particles.erase(particles.begin() + i);
	}
}

void Particle::Draw_Particles() {
	for (int i = particles.size() - 1; i > -1; i--)
		Particle::particles[i]->Draw();
}



#endif