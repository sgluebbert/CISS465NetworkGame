#include "Pickup.h"



Collectible::Collectible() {
    texture = surface_manager->smoke;
	//state = CREATED;
	type = Collectible_Type(rand() % 4);
}



void Collectible::OnPickup(Ship * ship) {
	switch (type) {
	case HEALTH_COLLECTIBLE:
		ship->health += ship->max_health * 0.5;

		if (ship->health > ship->max_health)
			ship->health = ship->max_health;

		break;
	case SHIELDS_COLLECTIBLE:
		ship->shields += ship->max_shields * 0.5;

		if (ship->shields > ship->max_shields)
			ship->shields = ship->max_shields;

		break;
	case ARMOR_COLLECTIBLE:
		ship->armor += ship->max_armor * 0.5;

		if (ship->armor > ship->max_armor)
			ship->armor = ship->max_armor;

		break;
	case POWER_COLLECTIBLE:
		ship->power += ship->max_power * 0.5;

		if (ship->power > ship->max_power)
			ship->power = ship->max_power;

		break;
	}
}

void Collectible::Update(Ship * ship) {
	
}

bool Collectible::OnExpiration(Ship * ship) {
	
}