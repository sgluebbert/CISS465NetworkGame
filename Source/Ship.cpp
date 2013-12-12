#include "Ship.h"



const float Ship::default_max_resource 		= 100.0;
const float Ship::default_recharge_delay 	=   2.5;
const float Ship::default_recharge_rate 	=  10.0;
const float Ship::default_capture_modifier 	=   1.0;

std::deque<Ship *> Ship::ships;
int Ship::ship_count = 0;



void Ship::Setup_Interceptor() {
	mass = 30.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 40.0;
    turn_rate = 50.0;

    texture = surface_manager->ship_interceptor;
    texture_c = surface_manager->ship_interceptor_c;

	max_health = max_shields = health = shields = default_max_resource * 0.75;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier * 4.0 / 3.0;

	weapon_pool[ENERGY_TYPE]	= new Laser(this);
	weapon_pool[BALLISTIC_TYPE] = new Gauss(this);
	weapon_pool[PROPELLED_TYPE] = new Rocket(this);
	weapon_pool[BOMB_TYPE]		= new Bomb(this);
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Fighter() {
	mass = 40.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 30.0;
    turn_rate = 40.0;

    texture = surface_manager->ship_fighter;
    texture_c = surface_manager->ship_fighter_c;

	max_health = max_shields = health = shields = default_max_resource;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier;

	weapon_pool[ENERGY_TYPE]	= new Laser(this);
	weapon_pool[BALLISTIC_TYPE] = new Gauss(this);
	weapon_pool[PROPELLED_TYPE] = new Rocket(this);
	weapon_pool[BOMB_TYPE]		= new Bomb(this);
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Frigate() {
	mass = 50.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 22.5;
    turn_rate = 30.0;

    texture = surface_manager->ship_frigate;
    texture_c = surface_manager->ship_frigate_c;

	max_health = max_shields = health = shields = default_max_resource * 4.0 / 3.0;
	max_armor = max_power = armor = power = default_max_resource;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier * 0.75;

	weapon_pool[ENERGY_TYPE]	= new Laser(this);
	weapon_pool[BALLISTIC_TYPE] = new Gauss(this);
	weapon_pool[PROPELLED_TYPE] = new Rocket(this);
	weapon_pool[BOMB_TYPE]		= new Bomb(this);
	weapon_pool[POWERUP_TYPE]	= NULL;
}

void Ship::Setup_Bomber() {
	mass = 40.0;
	Set_Inertia(bounding_volume.r);

    velocity_limit = 30.0;
    turn_rate = 40.0;

    texture = surface_manager->ship_bomber;
    texture_c = surface_manager->ship_bomber_c;

	max_health = max_shields = health = shields = default_max_resource * 0.75;
	max_armor = max_power = armor = power = default_max_resource * 4.0 / 3.0;
	shield_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_timer.Set_Interval(default_recharge_delay);
	power_recharge_rate = shield_recharge_rate = default_recharge_rate;
	capture_modifier = default_capture_modifier;

	weapon_pool[ENERGY_TYPE]	= new Laser(this);
	weapon_pool[BALLISTIC_TYPE] = new Gauss(this);
	weapon_pool[PROPELLED_TYPE] = new Rocket(this);
	weapon_pool[BOMB_TYPE]		= new Bomb(this);
	weapon_pool[POWERUP_TYPE]	= NULL;
}



Ship::Ship(Ship_Type ship_type, float _x, float _y) {
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    Collidable::objects.push_back(this);
    Set_Group(SHIP_GROUP);

	dx = dy = force = torque = velocity = rotation = 0.0;

	team_id = NO_TEAM;
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
	captures = 0;
	kills = 0;
	shots_hit = 0;

	smoke_emitter.Set_Particle(Create_Smoke_Particle());
	shatter_emitter.Set_Particle(Create_Shatter_Particle());

	//Configuration Methods
	smoke_emitter.Set_Spawn_Count(1);
	smoke_emitter.Set_Spawn_Delay(0.1);
	smoke_emitter.Set_Max_Age(1.0);
	smoke_emitter.Set_Starting_Angle(0.0);
	smoke_emitter.Set_Starting_Angle_Variance(360.0);
	shatter_emitter.Set_Spawn_Count(10);
	shatter_emitter.Set_Spawn_Delay(0.1);
	shatter_emitter.Set_Max_Age(1.0);
	shatter_emitter.Set_Starting_Angle(0.0);
	shatter_emitter.Set_Max_Age_Variance(0.25);
	shatter_emitter.Set_Starting_Angle_Variance(360.0);
	shatter_emitter.Set_Starting_Velocity_Variance(20.0);

	switch(ship_type) {
	case INTERCEPTOR:	Setup_Interceptor();	break;
	case FIGHTER:		Setup_Fighter();		break;
	case FRIGATE:		Setup_Frigate();		break;
	case BOMBER:		Setup_Bomber();			break;
	}
}


Ship::~Ship()
{
}


NetString * Ship::Serialize() {
	NetString * string = new NetString();
	if (!string->WriteFloat(x) || !string->WriteFloat(y) || !string->WriteFloat(angle) || !string->WriteFloat(health) || !string->WriteFloat(shields) || !string->WriteFloat(armor) || !string->WriteFloat(power) || !string->WriteUChar((unsigned char)state)) {
		delete string;
		return NULL;
	}

	return string;
}

bool Ship::Deserialize(NetString *string) {
	unsigned char temp1;
	if (!string->ReadFloat(x) || !string->ReadFloat(y) || !string->ReadFloat(angle) || !string->ReadFloat(health) || !string->ReadFloat(shields) || !string->ReadFloat(armor) || !string->ReadFloat(power) || !string->ReadUChar(temp1))
		return false;

	state = (Ship_State)temp1;

	return true;
}



bool Ship::Fire(Weapon_Type weapon_id, bool forced) {
	if (!forced) {
		if (weapon_id < ENERGY_TYPE)
			return false;
		if (weapon_id > POWERUP_TYPE)
			return false;
		if (weapon_pool[weapon_id] == NULL)
			return false;
		if (power < weapon_pool[weapon_id]->power)
			return false;
	}

	if (weapon_pool[weapon_id]->Fire(forced)) {
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
}

void Ship::Turn_Right(double dt) {
    angle -= turn_rate * dt;
    if (angle < 0)
        angle = 360 + angle;
}



void Ship::Damage_Armor(float damage) {
	if (damage < 0)
		return;

	if (armor > 0)
		armor -= damage;

	if (armor < 0.0) {
		damage = 0 - armor;
		armor = -0.1;
	}
	else {
		shatter_emitter.Set_Particle_Count(int(damage / 10) * 10);
		shatter_emitter.Activate();
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

	if (health < max_health * 0.5) {
		smoke_emitter.Set_Particle_Count(500 / health);
	    smoke_emitter.Activate();
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
	// Not efficient i know, but the server does not init the ship with a team, so for now...
	weapon_pool[ENERGY_TYPE]->team_id = team_id;
	weapon_pool[BALLISTIC_TYPE]->team_id = team_id;
	weapon_pool[PROPELLED_TYPE]->team_id = team_id;
	weapon_pool[BOMB_TYPE]->team_id = team_id;

	respawn_timer.Update(dt);

	smoke_emitter.Update(dt, x, y);
	shatter_emitter.Update(dt, x, y);

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

	    if (health <= 0.0) {
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
	    		weapon_pool[i]->Update(dt, angle, x, y);

		break;

	case DYING:
		state = DEAD;

		break;

	case DEAD:
		smoke_emitter.Deactivate();
		shatter_emitter.Deactivate();

		break;
	}
}

void Ship::Draw() {
    if (state == DEAD || state == DYING)
        return;
    if (texture == NULL)
        std::cout << "SHIP: I NEED A TEXTURE!!!" << std::endl;

	glColor4f(1.0, 1.0, 1.0, 1.0);
    texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);

    if (team_id == BLUE_TEAM)
		glColor4f(0.0, 0.0, 1.0, 0.5);
	else if (team_id == RED_TEAM)
		glColor4f(1.0, 0.0, 0.0, 0.5);

    texture_c->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
}

void Ship::Sync() {
	Entity::Sync();
	bounding_volume.x = x;
	bounding_volume.y = y;
}



int Ship::Add_Ship(Team _team, Ship_Type _type, float _x, float _y, float respawn_time) {
	if (ship_count >= Ship::ships.size())
		return -1;

	int i = 0;

	for (i; i < Ship::ships.size(); i++)
		if (ships[i] == NULL) {
			ships[i] = new Ship(_type, _x, _y);
			ships[i]->team_id = _team;
			ships[i]->respawn_timer.Set_Interval(respawn_time);
			break;
		}

	Ship::ship_count++;

	return i;
}

void Ship::Remove_Ship(int index) {
	if (index < 0)
		return;
	if (index > Ship::ships.size())
		return;

	if (Ship::ships[index] == NULL)
		return;

	for (std::deque<Drawable *>::iterator it = Drawable::objects.begin(); it != Drawable::objects.end(); it++)
	{
		if (*it == Ship::ships[index])
		{
			Drawable::objects.erase(it);
			break;
		}
	}

    for (std::deque<Rigid_Body *>::iterator it = Rigid_Body::objects.begin(); it != Rigid_Body::objects.end(); it++)
	{
		if (*it == Ship::ships[index])
		{
			Rigid_Body::objects.erase(it);
			break;
		}
	}

    for (std::deque<Collidable *>::iterator it = Collidable::objects.begin(); it != Collidable::objects.end(); it++)
	{
		Collidable *c = *it;
		Particle *p = static_cast<Particle *>(c);
		if (p != NULL)
		{
			if (p->owner == Ship::ships[index])
				p->owner = NULL;
		}

		if (*it == Ship::ships[index])
		{
			Collidable::objects.erase(it);
			// break;
		}
	}

	delete Ship::ships[index];
	Ship::ships[index] = NULL;
}

void Ship::Remove_Ship(Ship *ship) {
	for (int i = 0; i < Ship::ships.size(); i++)
	{
		if (Ship::ships[i] == ship)
		{
			Remove_Ship(i);
			return;
		}
	}
}

void Ship::Initialize_Ships(int max_number_of_players) {
	Ship::ships = std::deque<Ship *>(max_number_of_players);

	for (int i = 0; i < Ship::ships.size(); i++)
		Ship::ships[i] = NULL;

	Ship::ship_count = 0;
}

void Ship::Cleanup_Ships() {
	for (int i = 0; i < Ship::ships.size(); i++)
		if (Ship::ships[i] != NULL)
			delete Ship::ships[i];

	Ship::ships.clear();
	Ship::ship_count = 0;
}