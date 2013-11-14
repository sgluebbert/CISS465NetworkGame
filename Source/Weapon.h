#ifndef WEAPON_H
#define WEAPON_H



#include "Timer.h"
#include "Particles/Particle_Emitter.h"



enum Weapon_State 	{	WEAPON_READY, WEAPON_FIRING, WEAPON_RECHARGING, WEAPON_DISABLED		};



class Weapon {
public:
	Weapon_State state;
	Timer recharge_timer;
	Particle_Emitter emitter;

	float x, y;

	float damage, range, power;

	float shield_modifier, hull_modifier, armor_modifier, obstacle_modifier;

	bool Fire(bool forced = false);

	void Update(double, float, float);
};



class Laser : public Weapon {
public:
	Laser();
};

class Gauss : public Weapon {
public:
	Gauss();
};

class Rocket : public Weapon {
public:
	Rocket();
};

class Bomb : public Weapon {
public:
	Bomb();
};



#endif