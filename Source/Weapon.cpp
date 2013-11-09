#include "Weapon.h"



bool Weapon::Fire() {
	if (state != WEAPON_READY)
		return false;

	state = WEAPON_FIRING;
	return true;
}

void Weapon::Update(double dt) {
	switch(state) {
	case WEAPON_READY:

		break;

	case WEAPON_FIRING:
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
	
	emitter.Update(dt);
}



Laser::Laser() {
	shield_modifier = 4.0 / 3.0;
	hull_modifier = 0.75;
	armor_modifier = 0.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(1.5);

	damage = 5.0;
	range = 200.0;
	power = 5.0;
}

Gauss::Gauss() {
	shield_modifier = 0.75;
	hull_modifier = 4.0 / 3.0;
	armor_modifier = 0.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(4.5);
	
	damage = 15.0;
	range = 400.0;
	power = 30.0;
}

Rocket::Rocket() {
	shield_modifier = 1.0;
	hull_modifier = 1.0;
	armor_modifier = 1.0;
	obstacle_modifier = 1.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(3.0);
	
	damage = 10.0;
	range = 400.0;
	power = 20.0;
}

Bomb::Bomb() {
	shield_modifier = 1.0;
	hull_modifier = 1.0;
	armor_modifier = 0.0;
	obstacle_modifier = 4.0 / 3.0;

	state = WEAPON_READY;
	recharge_timer.Set_Interval(3.0);
	
	damage = 15.0;
	range = 100.0;
	power = 20.0;
}