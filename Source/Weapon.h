#ifndef WEAPON_H
#define WEAPON_H



#include "Timer.h"
#include "Particles/Particle_Emitter.h"
#include "Particles/ParticleManager.h"



enum Weapon_State 	{	WEAPON_READY, WEAPON_FIRING, WEAPON_RECHARGING, WEAPON_DISABLED		};


class Weapon {
public:
	Weapon_State state;
	Timer recharge_timer;
	Particle_Emitter emitter;
	Team team_id;
	Ship *owner;

	float x, y;

	float damage, range, power;

	float shield_modifier, hull_modifier, armor_modifier, obstacle_modifier;

	bool Fire(bool forced = false);

	void Update(double, float, float, float);
};



class Laser : public Weapon {
public:
	Laser(Ship *);
};

class Gauss : public Weapon {
public:
	Gauss(Ship *);
};

class Rocket : public Weapon {
public:
	Rocket(Ship *);
};

class Bomb : public Weapon {
public:
	Bomb(Ship *);
};



#endif