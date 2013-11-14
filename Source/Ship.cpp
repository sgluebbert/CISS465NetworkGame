#include "Ship.h"



const float Ship::default_max_resource 		= 100.0;
const float Ship::default_recharge_delay 	=   2.5;
const float Ship::default_recharge_rate 	=  10.0;
const float Ship::default_capture_modifier 	=   1.0;



void Ship::Setup_Interceptor() {
	mass = 30.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 40.0;
    turn_rate = 50.0;

	texture = surface_manager->ship;

	max_health = max_shields = health = shields = default_max_resource * 0.75;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier * 4.0 / 3.0;

	weapon_pool[ENERGY_TYPE]	= new Laser();
	weapon_pool[BALLISTIC_TYPE] = new Gauss();
	weapon_pool[PROPELLED_TYPE] = new Rocket();
	weapon_pool[BOMB_TYPE]		= new Bomb();
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Fighter() {
	mass = 40.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 30.0;
    turn_rate = 40.0;

	texture = surface_manager->ship;

	max_health = max_shields = health = shields = default_max_resource;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier;

	weapon_pool[ENERGY_TYPE]	= new Laser();
	weapon_pool[BALLISTIC_TYPE] = new Gauss();
	weapon_pool[PROPELLED_TYPE] = new Rocket();
	weapon_pool[BOMB_TYPE]		= new Bomb();
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Frigate() {
	mass = 50.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 22.5;
    turn_rate = 30.0;

	texture = surface_manager->ship;

	max_health = max_shields = health = shields = default_max_resource * 4.0 / 3.0;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier * 0.75;

	weapon_pool[ENERGY_TYPE]	= new Laser();
	weapon_pool[BALLISTIC_TYPE] = new Gauss();
	weapon_pool[PROPELLED_TYPE] = new Rocket();
	weapon_pool[BOMB_TYPE]		= new Bomb();
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Bomber() {
	mass = 40.0;
	Set_Inertia(bounding_volume.r);

	//Motor Variables
    velocity_limit = 30.0;
    turn_rate = 40.0;

	//Ship Variables
	texture = surface_manager->ship;

	max_health = max_shields = health = shields = default_max_resource * 0.75;
	max_armor = max_power = armor = power = default_max_resource * 4.0 / 3.0;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier;

	weapon_pool[ENERGY_TYPE]	= new Laser();
	weapon_pool[BALLISTIC_TYPE] = new Gauss();
	weapon_pool[PROPELLED_TYPE] = new Rocket();
	weapon_pool[BOMB_TYPE]		= new Bomb();
	weapon_pool[POWERUP_TYPE]	= NULL;
}



Ship::Ship(Ship_Type ship_type, float _x, float _y) {
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    Collidable::objects.push_back(this);

	dx = dy = force = torque = velocity = rotation = 0.0;

	x = bounding_volume.x = _x;
	y = bounding_volume.y = _y;
	draw_scale = bounding_volume.r = 32.0;
	draw_angle = angle = 0.0;
	drawing_box.x = x - bounding_volume.r;
	drawing_box.y = y - bounding_volume.r;
	drawing_box.w = 2 * bounding_volume.r;
	drawing_box.h = 2 * bounding_volume.r;

    rotation_limit = 0.0;
    reverse_modifier = 0.5;
    force_motor = torque_motor = 0.0;

	state = ALIVE;

	/*smoke_emitter.Set_Particle(Create_Smoke_Particle());

	//Configuration Methods
	smoke_emitter.Set_Particle_Count(50);
	smoke_emitter.Set_Spawn_Count(1);
	smoke_emitter.Set_Spawn_Delay(0.1);
	smoke_emitter.Set_Max_Age(1.0);
	smoke_emitter.Set_Starting_Angle(-draw_angle);
	smoke_emitter.Set_Starting_Angle_Variance(45.0);*/

	switch(ship_type) {
	case INTERCEPTOR:	Setup_Interceptor();	break;
	case FIGHTER:		Setup_Fighter();		break;
	case FRIGATE:		Setup_Frigate();		break;
	case BOMBER:		Setup_Bomber();			break;
	}
}



NetString * Ship::Serialize() {
	NetString * string = new NetString();
	
	if (!string->WriteFloat(x) || !string->WriteFloat(y) || !string->WriteFloat(angle) || !string->WriteFloat(health)) {
		delete string;
		return NULL;
	}

	return string;
}

bool Ship::Deserialize(NetString *string) {
	if (!string->ReadFloat(x) || !string->ReadFloat(y) || !string->ReadFloat(angle) || !string->ReadFloat(health))
		return false;

	return true;
}



bool Ship::Fire(Weapon_Type weapon_id) {
	if (weapon_id < ENERGY_TYPE)
		return false;
	if (weapon_id > POWERUP_TYPE)
		return false;
	if (weapon_pool[weapon_id] == NULL)
		return false;
	if (power < weapon_pool[weapon_id]->power)
		return false;

	if (weapon_pool[weapon_id]->Fire()) {
		power -= weapon_pool[weapon_id]->power;
		power_recharge_timer.Start();
		return true;
	}

	return false;
}

void Ship::Accelerate(bool reverse) {
	if (!reverse)
		force_motor = 2000.0;
	else
		force_motor = -2000.0;
}

void Ship::Decelerate() {
	force_motor = 0.0;
}

void Ship::Turn_Left(double dt) {
    angle += turn_rate * dt;
    if (angle >= 360)
        angle = angle - 360;

	smoke_emitter.Set_Starting_Angle(-angle);
}

void Ship::Turn_Right(double dt) {
    angle -= turn_rate * dt;
    if (angle < 0)
        angle = 360 + angle;

	smoke_emitter.Set_Starting_Angle(-angle);
}



void Ship::Damage_Armor(float damage) {
	if (damage < 0)
		return;

	if (armor > 0)
		armor -= damage;

	if (armor < 0) {
		damage = 0 - armor;
		armor = 0;
	}
}

void Ship::Damage_Shields(float damage) {
	if (damage < 0)
		return;

	shield_recharge_timer.Start();

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

void Ship::Damage_Hull(float damage) {
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



void Ship::Limit_Motor() {
    if (rotation > rotation_limit)
        rotation = rotation_limit;
    
    if (velocity > velocity_limit)
        velocity = velocity_limit;

    if (velocity < -velocity_limit * reverse_modifier)
        velocity = -velocity_limit * reverse_modifier;
}

void Ship::Update(double dt) {
	respawn_timer.Update(dt);

	smoke_emitter.Update(dt, x, y);
	explosion_emitter.Update(dt, x, y);

	switch(state) {
	case ALIVE:
	    velocity *= FRICTION_COEFFICIENT;
	    rotation *= FRICTION_COEFFICIENT;

	    Apply_Force(force_motor, angle, 0, 0);
	    Apply_Torque(torque_motor);

	    Calculate_Velocity(dt);
	    Calculate_Rotation(dt);

	    Limit_Motor();
	    
	    Calculate_Vector(dt);
	    Move(dt);

	    bounding_volume.Update(dx, -dy);
	    drawing_box.Update(dx, -dy);
	    draw_angle = angle;

	    /*if (health <= 0.5 * max_health)
	        smoke_emitter.Activate();*/

	    if (health <= 0.0) {
	        smoke_emitter.Activate();
	        state = DYING;
			respawn_timer.Start();
	    }

		shield_recharge_timer.Update(dt);
		power_recharge_timer.Update(dt);

		if (shields < max_shields)
			if (shield_recharge_timer.Ended())
				shields += shield_recharge_rate * dt;

		if (shields > max_shields)
			shields = max_shields;

		if (power < max_power)
			if (power_recharge_timer.Ended())
				power += power_recharge_rate * dt;

		if (power > max_power)
			power = max_power;

	    for (int i = 0; i < 4; i++)
	    	if (weapon_pool[i] != NULL)
	    		weapon_pool[i]->Update(dt, x, y);

		break;

	case DYING:
		if (!smoke_emitter.Is_Active())
			state = DEAD;

		break;

	case DEAD:
		smoke_emitter.Deactivate();
		explosion_emitter.Deactivate();

		break;
	}
}

void Ship::Draw() {
    if (state == DEAD || state == DYING)
        return;
    if (texture == NULL)
        std::cout << "SHIP: I NEED A TEXTURE!!!" << std::endl;

    if (team_id == 0)
		glColor4f(0.0, 0.0, 1.0, 1.0);
	else if (team_id == 1)
		glColor4f(1.0, 0.0, 0.0, 1.0);

    texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
}



