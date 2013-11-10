#ifndef SHIP_H
#define SHIP_H



#include "Networking/NetString.h"
#include "Particle_Emitter.h"
#include "Texture.h"
#include "Weapon.h"



enum Ship_State	 {	ALIVE, DYING, DEAD	};
enum Weapon_Type {	ENERGY_TYPE, BALLISTIC_TYPE, PROPELLED_TYPE, BOMB_TYPE, POWERUP_TYPE	};



class Ship : public Entity {
public:
    void Set_Texture(Texture *, float, float);

    NetString * Serialize();
    bool Deserialize(NetString *);

	void Fire(Weapon_Type);
	void Accelerate(bool);
	void Decelerate();

	void Damage_Armor(float);
	void Damage_Shields(float);
	void Damage_Hull(float);

	void Update(double);
	void Draw();

//protected:
	static const float default_max_resource, default_power_recharge, default_capture_modifier;

    Texture * texture;
    Particle_Emitter smoke_emitter;
    Particle_Emitter explosion_emitter;

    Ship_State state;

    Timer respawn_timer;

	float max_health, max_shields, max_armor, max_power;
	float health, shields, armor, power;
	float shield_recharge_delay, shield_recharge_rate;
	float power_recharge_rate;
	float capture_modifier;
	Weapon * weapon_pool[5];
};



class Interceptor : public Ship {
public:
	Interceptor(double _x, double _y) {
		//Entity Variables
		x = _x;
		y = _y;
		w = 20;
		angle = 0.0;
		mass = 30.0;

		//Dynamic Entity Variables
	    max_velocity = 40.0;
	    reverse_modifier = 0.5;
	    turn_rate = 50.0;

		//Ship Variables
    	texture = NULL;
		state = ALIVE;

		max_health = max_shields = health = shields = default_max_resource * 0.75;
		max_armor = max_power = armor = power = default_max_resource;
		power_recharge_rate = default_power_recharge;
		capture_modifier = default_capture_modifier * 4.0 / 3.0;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 32, 32);

		weapon_pool[ENERGY_TYPE]	= new Laser();
		weapon_pool[BALLISTIC_TYPE] = new Gauss();
		weapon_pool[PROPELLED_TYPE] = new Rocket();
		weapon_pool[BOMB_TYPE]		= new Bomb();
		weapon_pool[POWERUP_TYPE]	= NULL;
	}

private:
};



class Fighter : public Ship {
public:
	Fighter(double _x, double _y) {
		//Entity Variables
		x = _x;
		y = _y;
		angle = 0.0;
		mass = 40.0;

		//Dynamic Entity Variables
	    max_velocity = 30.0;
	    reverse_modifier = 0.5;
	    turn_rate = 40.0;

		//Ship Variables
    	texture = NULL;
		state = ALIVE;

		max_health = max_shields = health = shields = default_max_resource;
		max_armor = max_power = armor = power = default_max_resource;
		power_recharge_rate = default_power_recharge;
		capture_modifier = default_capture_modifier;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 32, 32);

		weapon_pool[ENERGY_TYPE]	= new Laser();
		weapon_pool[BALLISTIC_TYPE] = new Gauss();
		weapon_pool[PROPELLED_TYPE] = new Rocket();
		weapon_pool[BOMB_TYPE]		= new Bomb();
		weapon_pool[POWERUP_TYPE]	= NULL;
	}

private:
};



class Frigate : public Ship {
public: 
	Frigate(double _x, double _y) {
		//Entity Variables
		x = _x;
		y = _y;
		angle = 0.0;
		mass = 50.0;

		//Dynamic Entity Variables
	    max_velocity = 22.5;
	    reverse_modifier = 0.5;
	    turn_rate = 30.0;

		//Ship Variables
    	texture = NULL;
		state = ALIVE;

		max_health = max_shields = health = shields = default_max_resource * 4.0 / 3.0;
		max_armor = max_power = armor = power = default_max_resource;
		power_recharge_rate = default_power_recharge;
		capture_modifier = default_capture_modifier * 0.75;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 32, 32);

		weapon_pool[ENERGY_TYPE]	= new Laser();
		weapon_pool[BALLISTIC_TYPE] = new Gauss();
		weapon_pool[PROPELLED_TYPE] = new Rocket();
		weapon_pool[BOMB_TYPE]		= new Bomb();
		weapon_pool[POWERUP_TYPE]	= NULL;
	}

private:
};



class Bomber : public Ship {
public:
	Bomber(double _x, double _y) {
		//Entity Variables
		x = _x;
		y = _y;
		angle = 0.0;
		mass = 40.0;

		//Dynamic Entity Variables
	    max_velocity = 30.0;
	    reverse_modifier = 0.5;
	    turn_rate = 40.0;

		//Ship Variables
    	texture = NULL;
		state = ALIVE;

		max_health = max_shields = health = shields = default_max_resource * 0.75;
		max_armor = max_power = armor = power = default_max_resource * 4.0 / 3.0;
		power_recharge_rate = default_power_recharge;
		capture_modifier = default_capture_modifier;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 32, 32);

		weapon_pool[ENERGY_TYPE]	= new Laser();
		weapon_pool[BALLISTIC_TYPE] = new Gauss();
		weapon_pool[PROPELLED_TYPE] = new Rocket();
		weapon_pool[BOMB_TYPE]		= new Bomb();
		weapon_pool[POWERUP_TYPE]	= NULL;
	}

private:
};



#endif