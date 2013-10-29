#ifndef SHIP_H
#define SHIP_H



class Ship : public Dynamic_Entity {
public:
	void Fire(int);

	void Update(double);
	void Draw();

protected:
	double max_health, max_shields, max_armor, max_power;
	double health, shields, armor, power;
	double capture_modifier; //Affects the rate defined by the planet
	//Weapons * weapon_pool;

	const static int NUMBER_OF_WEAPONS = 4;
};



void Ship::Fire(int weapon_id) {
	//weapon_pool[weapond_id]->Fire();
}

void Ship::Update(double dt) {
	Dynamic_Entity::Update(dt);
}

void Ship::Draw() {
	//Do stuff
}



class Interceptor : public Ship {
public:
	Interceptor() {
		max_health = max_shields = health = shields = 75.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 4.0 / 3.0;
	}

private:
};



class Fighter : public Ship {
public:
	Fighter() {
		max_health = max_shields = health = shields = 100.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 1.0;
	}

private:
};



class Frigate : public Ship {
public: 
	Frigate() {
		max_health = max_shields = health = shields = 400.0 / 3.0;
		max_armor = max_power = armor = power = 100.0;
		capture_modifier = 0.75;
	}

private:
};



class Bomber : public Ship {
public:
	Bomber() {
		max_health = max_shields = health = shields = 75.0;
		max_armor = max_power = armor = power = 400.0 / 3.0;
		capture_modifier = 1.0;
	}

private:
};



#endif