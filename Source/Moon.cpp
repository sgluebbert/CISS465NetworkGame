#include "Moon.h"


Moon::Moon(Team t, float _x, float _y, float _m, float _r, float fr)
{
	Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);

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
	boost_factor = 1.0;
	alive = true;

	team_id = t;
	mass = _m;
	dx = dy = force = torque =velocity = rotation = 0.0;
    
    moon = surface_manager->moon;
    field = surface_manager->field_glow_unlocked;

    x = bounding_volume.x = _x;
    y = bounding_volume.y = _y;
    draw_angle = angle = 0.0;
    draw_scale = bounding_volume.r = _r;
    drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
    drawing_box.w = 2 * bounding_volume.r;
    drawing_box.h = 2 * bounding_volume.r;
}

void Moon::Move()
{

}

void Moon::TakeDamage()
{
	
}

void Moon::DrawGravityField()
{
    if (team_id == BLUE_TEAM)
    {
        glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
    }
    else if (team_id == RED_TEAM)
    {
        glColor4f(1.0, 0.5f, 0.5f, 0.5f);
    }
    else if (team_id == NEUTRAL_TEAM)
    {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    }

    field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
}

void Moon::Draw()
{
  if (moon == NULL || alive == false)
        return;

    DrawGravityField();
   
   	if (type == HEALTH)
	{
		glColor4f(1.0, 1.0, 0.0, 0.5);
	}
	else if (type == SHIELD)
	{
		glColor4f(0.0, 1.0, 1.0, 0.5);
	}	
	else if (type == POWER)
	{
		glColor4f(1.0, 0.5, 1.0, 0.5);
	}
	else if (type == ARMOR)
	{
		glColor4f(0.0, 1.0, 0.0, 0.5);
	}

    moon->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale - field_radius);
    glColor4f(1.0, 1.0, 1.0, 1.0);
}

void Moon::Update(double dt)
{
 	Entity::Update(dt);
    drawing_box.Update(dx, -dy);
    bounding_volume.Update(dx, -dy);
    DistributeResource(dt);
}

void Moon::DistributeResource(double dt)
{
	for (int i = 0; i < Ship::ships.size(); i++)
		if (Ship::ships[i] != NULL)
			if (Ship::ships[i]->team_id == team_id)
		    {
		    	Ship * ship = Ship::ships[i];
		    	if (type == HEALTH)
				{
					if (ship->health < ship->max_health)
						ship->health += 5.0 * boost_factor * dt;

					if (ship->health > ship->max_health)
						ship->health = ship->max_health;
				}
				else if (type == SHIELD)
				{
					if (ship->shields < ship->max_shields)
						ship->shields += 5.0 * boost_factor * dt;
					
					if (ship->shields > ship->max_shields)
						ship->shields = ship->max_shields;
				}	
				else if (type == POWER)
				{
					if (ship->power < ship->max_power)
						ship->power += 5.0 * boost_factor * dt;
					
					if (ship->power > ship->max_power)
						ship->power = ship->max_power;

				}
				else if (type == ARMOR)
				{
					if (ship->armor < ship->max_armor)
						ship->armor += 5.0 * boost_factor * dt;
					
					if (ship->armor > ship->max_armor)
						ship->armor = ship->max_armor;
				}
		    }
}