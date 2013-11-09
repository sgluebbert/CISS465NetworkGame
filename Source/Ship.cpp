#include "Ship.h"



double Ship::default_max_resource = 100.0;
double Ship::default_power_recharge = 0.5;
double Ship::default_capture_modifier = 1.0;



void Ship::Set_Texture(Texture * tex, float width, float height) {
    texture = tex;
    w = width;
    h = height;
}

NetString *Ship::Serialize() {
	NetString * string = new NetString();

	NetString * baseString = Entity::Serialize();
	if (baseString == NULL)
		return NULL;

	if (!(*string += *baseString)) {
		delete string;
		return NULL;
	}

	if (!string->WriteFloat(health)) {
		delete string;
		return NULL;
	}

	return string;
}

bool Ship::Deserialize(NetString *string) {
	if (!Entity::Deserialize(string))
		return false;

	if (!string->ReadFloat(health))
		return false;

	return true;
}

void Ship::Fire(Weapon_Type weapon_id) {
	if (weapon_id < ENERGY_TYPE)
		return;
	if (weapon_id > POWERUP_TYPE)
		return;
	if (weapon_pool[weapon_id] == NULL)
		return;
	if (power < weapon_pool[weapon_id]->power)
		return;

	if (weapon_pool[weapon_id]->Fire())
		power -= weapon_pool[weapon_id]->power;
}

void Ship::Damage_Armor(double damage) {
	if (damage < 0)
		return;

	if (armor > 0)
		armor -= damage;

	if (armor < 0) {
		damage = 0 - armor;
		armor = 0;
	}
}

void Ship::Damage_Shields(double damage) {
	if (damage < 0)
		return;

	if (shields > 0)
		shields -= damage;
	else {
		Damage_Hull(damage);
		return;
	}

	if (shields < 0) {
		damage = 0 - shields;
		shields = 0;
	}
	else
		return;

	Damage_Hull(damage);
}

void Ship::Damage_Hull(double damage) {
	damage /= (1.0 + (1.0 * armor / 100.0));

	if (health > 0)
		health -= damage;
	else {
		state = DYING;
		return;
	}

	if (health < 0) {
		damage = 0 - health;
		health = 0;
	}
	else
		return;
	
	if (health < 0)
		state = DYING;
}

void Ship::Update(double dt) {
	Entity::Update(dt);

	smoke_emitter.Update(dt);
	explosion_emitter.Update(dt);

	switch(state) {
	case ALIVE:
	    if (health <= 0.5 * max_health)
	        smoke_emitter.Activate();

	    if (health <= 0.0) {
	        explosion_emitter.Activate();
	        state = DYING;
	    }

	    if (power < max_power)
	    	power += power_recharge_rate;

	    if (power > max_power)
	    	power = max_power;

	    for (int i = 0; i < 4; i++)
	    	if (weapon_pool[i] != NULL)
	    		weapon_pool[i]->Update(dt);

		break;

	case DYING:
		if (explosion_emitter.Is_Active())
			state = DEAD;

		break;

	case DEAD:
		smoke_emitter.Deactivate();
		explosion_emitter.Deactivate();

		break;
	}
}

void Ship::Draw() {
    if (texture == NULL)
        return;
    if (state == DEAD || state == DYING)
        return;

	glColor4f(1.0, 1.0, 1.0, 1.0);
    texture->DrawCentered(x, y, -angle, 40);
}



