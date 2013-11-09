#ifndef WEAPON_H
#define WEAPON_H



#include "Timer.h"
#include "Particle_Emitter.h"



enum Weapon_State 	{	WEAPON_READY, WEAPON_FIRING, WEAPON_RECHARGING, WEAPON_DISABLED		};



class Weapon {
public:
	Weapon_State state;
	Timer recharge_timer;
	Particle_Emitter emitter;

	double damage;
	double range;
	double power;

	double shield_modifier;
	double hull_modifier;
	double armor_modifier;
	double obstacle_modifier;



	bool Fire();

	void Update(double);
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