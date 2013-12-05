#include "Moon.h"


Moon::Moon(float _offset, float _angle, float _ocx, float _ocy, float _m, float _r, float fr)
{
	Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    Set_Group(MOON_GROUP);

	int type_num = rand() % 4;
	switch (type_num)
	{
		case 0: type = HEALTH; break;
		case 1: type = SHIELD; break;
		case 2: type = POWER; break;
		case 3: type = ARMOR; break;
		default: type = HEALTH; break;
	}

	field_radius = fr;
	health = max_health = 500.0;
	alignment = 0.0;
	capture_rate = 0.025;
	boost_factor = 1.0;
	alive = true;

	team_id = NEUTRAL_TEAM;
	mass = _m;
	dx = dy = force = torque = velocity = rotation = 0.0;
    
    moon = surface_manager->moon;
    field = surface_manager->field_glow_unlocked;

    orbit_center_x = _ocx;
    orbit_center_y = _ocy;
    orbit_radius = _offset;

    draw_angle = angle = _angle;
    x = bounding_volume.x = orbit_center_x + orbit_radius * sin(angle*PI/180);
	y = bounding_volume.y = orbit_center_y + orbit_radius * cos(angle*PI/180);

    draw_scale = bounding_volume.r = _r + fr;
    drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
    drawing_box.w = 2 * bounding_volume.r;
    drawing_box.h = 2 * bounding_volume.r;
}

void Moon::TakeDamage()
{
	
}

void Moon::DrawGravityField()
{
    if (field == NULL)
        return;

    if (alignment > 0.0)
        glColor4f(0.5, 0.5, 0.5 + alignment / 2.0, 1.0);
    else if (alignment < 0.0f)
        glColor4f(0.5 - alignment / 2.0, 0.5, 0.5, 1.0);
    else
        glColor4f(0.5, 0.5, 0.5, 1.0);

    field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
}

void Moon::Draw()
{
  if (moon == NULL || alive == false)
        return;

    DrawGravityField();
   
   	if (type == HEALTH)
	{
		glColor4f(1.0, 1.0, 0.0, 1.0);
	}
	else if (type == SHIELD)
	{
		glColor4f(0.0, 1.0, 1.0, 1.0);
	}	
	else if (type == POWER)
	{
		glColor4f(1.0, 0.5, 1.0, 1.0);
	}
	else if (type == ARMOR)
	{
		glColor4f(0.0, 1.0, 0.0, 1.0);
	}

    moon->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale - field_radius);
    glColor4f(1.0, 1.0, 1.0, 1.0);
}

void Moon::Update(double dt)
{
	angle += 2.5f * dt;

	if (angle > 360)
		angle -= 360;

	if (angle < 0)
		angle += 360;

	x = orbit_center_x + orbit_radius * sin(angle*PI/180);
	y = orbit_center_y + orbit_radius * cos(angle*PI/180);

	drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
 	bounding_volume.x = x;
    bounding_volume.y = y;

 	Entity::Update(dt);
    drawing_box.Update(dx, -dy);
    bounding_volume.Update(dx, -dy);
    DistributeRegen(dt);
}

void Moon::DistributeResource()
{
    Ship * ship = NULL;

    for (int i = 0; i < Ship::ships.size(); i++) {
    	ship = Ship::ships[i];

    	if (ship != NULL)
	        if (ship->team_id == team_id)
	            switch(type) {
	            case HEALTH:
	                ship->max_health  *= 1.5 * boost_factor;
	                ship->health      += ship->max_health * 1.5 * boost_factor;
	                break;

	            case SHIELD:
	                ship->max_shields *= 1.5 * boost_factor;
	                ship->shields     += ship->max_shields * 1.5 * boost_factor;
	                break;

	            case POWER: 
	                ship->max_armor   *= 1.5 * boost_factor;
	                ship->armor       += ship->max_armor * 1.5 * boost_factor;
	                break;

	            case ARMOR: 
	                ship->max_power   *= 1.5 * boost_factor;
	                ship->power       += ship->max_power * 1.5 * boost_factor;
	                break;
	            }
    }
}

void Moon::RemoveResource(Team _team)
{
    Ship * ship = NULL;

    for (int i = 0; i < Ship::ships.size(); i++) {
    	ship = Ship::ships[i];

    	if (ship != NULL)
	        if (ship->team_id == team_id)
	            switch(type) {
	            case HEALTH:
	                ship->health      -= ship->max_health * 1.5 * boost_factor;
	                ship->max_health  /= 1.5 * boost_factor;
	                break;

	            case SHIELD:
	                ship->shields     -= ship->max_shields * 1.5 * boost_factor;
	                ship->max_shields /= 1.5 * boost_factor;
	                break;

	            case POWER: 
	                ship->armor       -= ship->max_armor * 1.5 * boost_factor;
	                ship->max_armor   /= 1.5 * boost_factor;
	                break;

	            case ARMOR: 
	                ship->power       -= ship->max_power * 1.5 * boost_factor;
	                ship->max_power   /= 1.5 * boost_factor;
	                break;
	            }
	}
}

void Moon::DistributeRegen(double dt)
{
    Ship * ship = NULL;

	for (int i = 0; i < Ship::ships.size(); i++) {
		ship = Ship::ships[i];

		if (ship != NULL)
			if (ship->team_id == team_id)
	            switch(type) {
	            case HEALTH:
					if (ship->health < ship->max_health)
						ship->health += 5.0 * boost_factor * dt;

					if (ship->health > ship->max_health)
						ship->health = ship->max_health;
	                break;

	            case SHIELD:
					if (ship->shields < ship->max_shields)
						ship->shields += 5.0 * boost_factor * dt;
					
					if (ship->shields > ship->max_shields)
						ship->shields = ship->max_shields;
	                break;

	            case POWER: 
					if (ship->power < ship->max_power)
						ship->power += 5.0 * boost_factor * dt;
					
					if (ship->power > ship->max_power)
						ship->power = ship->max_power;
	                break;

	            case ARMOR: 
					if (ship->armor < ship->max_armor)
						ship->armor += 5.0 * boost_factor * dt;
					
					if (ship->armor > ship->max_armor)
						ship->armor = ship->max_armor;
	                break;

	            }
	}
}