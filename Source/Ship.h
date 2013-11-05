#ifndef SHIP_H
#define SHIP_H



#include "Particle_Emitter.h"
#include "Texture.h"



enum Ship_State {	ALIVE, DYING, DEAD	};



class Ship : public Entity {
public:

    void Set_Texture(Texture *, float, float);

	void Fire(int);

	void Update(double);
	void Draw();

//protected:
    Texture * texture;
    Particle_Emitter smoke_emitter;
    Particle_Emitter explosion_emitter;

    Ship_State state;

	double max_health, max_shields, max_armor, max_power;
	double health, shields, armor, power;
	double capture_modifier; //Affects the rate defined by the planet
	//Weapons * weapon_pool;

	const static int NUMBER_OF_WEAPONS = 4;
};



class Interceptor : public Ship {
public:
	Interceptor(double _x, double _y) {
		//Entity Variables
		x = _x;
		y = _y;
		angle = 0.0;
		mass = 30.0;

		//Dynamic Entity Variables
	    max_velocity = 40.0;
	    reverse_modifier = 0.5;
	    turn_rate = 50.0;

		//Ship Variables
    	texture = NULL;
		state = ALIVE;

		max_health = max_shields = health = shields = 75.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 4.0 / 3.0;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 64, 64);
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

		max_health = max_shields = health = shields = 100.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 1.0;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 64, 64);
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

		max_health = max_shields = health = shields = 400.0 / 3.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 0.75;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 64, 64);
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

		max_health = max_shields = health = shields = 75.0;
		max_armor = max_power = armor = power = 400.0 / 3.0;
		capture_modifier = 1.0;

		//Set Important Variables
		Set_Texture(surface_manager->ship, 64, 64);
	}

private:
};



#endif