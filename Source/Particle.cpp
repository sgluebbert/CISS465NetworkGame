#include "Particle.h"



std::vector<Particle *> Particle::particles;
//Quad_Tree<Particle *> Particle::particle_tree;



Particle::Particle() {
	age = 0;
	max_age = 0;
}

Particle::Particle(Particle & p) {
    dx = dy = 0.0;
	age = 0;

    bounding_volume = p.bounding_volume;

    x = p.x;
    y = p.y;
    w = p.w;
    h = p.h;
    angle = p.angle;

    velocity = p.velocity;
    max_velocity = p.max_velocity;
    reverse_modifier = p.reverse_modifier;
    force = p.force;
    mass = p.mass;
    inertia = p.inertia;
    turn_rate = p.turn_rate;

	max_age = p.max_age;
}

Particle::~Particle() {
}

bool Particle::Is_Dead() {
	return (age >= max_age);
}



void Particle::Update(double dt) {
    age += dt;

	Entity::Update(dt);
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