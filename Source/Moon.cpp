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
}

void Moon::DistributeResource(Ship * ship)
{
	if (ship->team_id == team_id)
    {
    	if (type == HEALTH)
		{

		}
		else if (type == SHIELD)
		{
			
		}	
		else if (type == POWER)
		{
			
		}
		else if (type == ARMOR)
		{
			
		}
    }
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
    