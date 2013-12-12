#include "Weapon.h"



bool Weapon::Fire(bool forced) {
	if (state != WEAPON_READY && !forced)
		return false;

	state = WEAPON_FIRING;
	return true;
}

void Weapon::Update(double dt, float _a, float _x, float _y) {
	x = _x;
	y = _y;
	
	switch(state) {
	case WEAPON_READY:

		break;

	case WEAPON_FIRING:
		emitter.Set_Starting_Angle(_a);
		emitter.Activate();

		state = WEAPON_RECHARGING;
		recharge_timer.Start();

		break;

	case WEAPON_RECHARGING:
		recharge_timer.Update(dt);

		if (recharge_timer.Ended())
			state = WEAPON_READY;

		break;

	case WEAPON_DISABLED:

		break;
	}
	
	// See ship update to understand this
	emitter.Set_Weapon_Team(team_id);

	emitter.Update(dt, x, y);
}



Laser::Laser() {
	shield_modifier = 4.0 / 3.0;
	hull_modifier = 0.75;
	armor_modifier = 0.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(0.75);

	damage = 20.0;
	range = 200.0;
	power = 5.0;

	emitter.Set_Particle(Create_Laser_Particle());
	emitter.Set_Particle_Count(1);
	emitter.Set_Spawn_Count(1);
	emitter.Set_Spawn_Delay(0.1);
	emitter.Set_Range(range);
	emitter.weapon_damage = damage;
}

Gauss::Gauss() {
	shield_modifier = 0.75;
	hull_modifier = 4.0 / 3.0;
	armor_modifier = 0.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(4.5);
	
	damage = 60.0;
	range = 400.0;
	power = 30.0;

	emitter.Set_Particle(Create_Gauss_Particle());
	emitter.Set_Particle_Count(1);
	emitter.Set_Spawn_Count(1);
	emitter.Set_Spawn_Delay(0.1);
	emitter.Set_Range(range);
	emitter.weapon_damage = damage;
}

Rocket::Rocket() {
	shield_modifier = 1.0;
	hull_modifier = 1.0;
	armor_modifier = 1.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(3.0);
	
	damage = 40.0;
	range = 400.0;
	power = 20.0;

	emitter.Set_Particle(Create_Rocket_Particle());
	emitter.Set_Particle_Count(1);
	emitter.Set_Spawn_Count(1);
	emitter.Set_Spawn_Delay(0.1);
	emitter.Set_Range(range);
	emitter.weapon_damage = damage;
}

Bomb::Bomb() {
	shield_modifier = 1.0;
	hull_modifier = 1.0;
	armor_modifier = 0.0;
	obstacle_modifier = 4.0 / 3.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(3.0);
	
	damage = 60.0;
	range = 100.0;
	power = 20.0;

	emitter.Set_Particle(Create_Mine_Particle());
	emitter.Set_Particle_Count(1);
	emitter.Set_Spawn_Count(1);
	emitter.Set_Spawn_Delay(0.1);
	emitter.Set_Range(range);
	emitter.weapon_damage = damage;
}