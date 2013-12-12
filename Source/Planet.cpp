#include "Planet.h"



std::list<Planet *> Planet::planet_graph;
float Planet::field_modifier = 1.0;



Planet::Planet(Team _id, float _x, float _y, float _m, float _r, float _cr)
    : gravity_radius(_r * field_modifier), capture_rate(_cr), just_captured(false)
{
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);

    Set_Group(PLANET_GROUP);

    team_id = _id;

    switch (team_id) {
        case RED_TEAM:
            alignment = -1.0;
            Lock(true);
            texture = surface_manager->red_planet;
            break;
        case BLUE_TEAM:
            alignment = 1.0;
            Lock(true);
            texture = surface_manager->blue_planet;
            break;
        default:
            alignment = 0.0;
            Lock(false);
            texture = surface_manager->neutral_planet;
            break;
    }

    dx = dy = force = torque = velocity = rotation = 0.0;
    
    field = surface_manager->planet_glow;

    x = bounding_volume.x = _x;
    y = bounding_volume.y = _y;
    draw_angle = angle = 0.0;
    draw_scale = bounding_volume.r = _r * (1.0 + field_modifier);
    drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
    drawing_box.w = 2 * bounding_volume.r;
    drawing_box.h = 2 * bounding_volume.r;

    mass = _m;
    Set_Inertia(_r);
}

Planet::~Planet()
{

}

void Planet::Lock(bool lock) {
    locked = lock;

    if (locked) {
        field = surface_manager->field_glow_locked;

        if (alignment > 0)
            alignment = 1.0;
        else// if (alignment < 0)
            alignment = -1.0;
    }
    else
        field = surface_manager->field_glow_unlocked;
}

void Planet::Update(double dt)
{
    just_captured = false;
    Entity::Update(dt);
    drawing_box.Update(dx, -dy);
    bounding_volume.Update(dx, -dy);
}

void Planet::Draw() 
{
    /*Draw Gravity Field*/
    //////////////////////////////////////////////////
    if (field == NULL)
        return;

    if (alignment > 0.0f)
    {
        glColor4f(0.5f, 0.5f, 0.5f + alignment/2.0f, 1.0f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
    else if (alignment < 0.0f)
    {
        glColor4f(0.5f + -alignment/2.0f, 0.5f, 0.5f, 1.0f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
    else
    {
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
    //////////////////////////////////////////////////


    /*Draw Actual Planet*/
    //////////////////////////////////////////////////
    if (texture == NULL)
        return;

    glColor4f(1.0, 1.0, 1.0, 1.0);
    texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale - gravity_radius);
    //////////////////////////////////////////////////
}



void Planet::Generate_Moons(float scale)
{
  
    int num_moons = (rand() % 4) + 1;

    float moon_mass = mass / 2.0f;
    float moon_size = scale / 2.0f;

    std::vector<float>start_angles;

    Moon * moon = NULL;
    for (int i = 0; i < num_moons; i++) {
        float offset = rand() % 300 + 200;
        float start_angle = rand() % 360;

        while (1)
        {
            bool flag = 0;
            for (int i = 0; i < start_angles.size();i++)
            {
                if (start_angle == start_angles[i])
                {
                    flag = 1;
                    break;
                }
            }

            if (flag == 1)
            {
                start_angle = rand() % 300 + 30;
            }
            else
            {
                for (int i = start_angle - 30; i < start_angle + 30; i++)
                {
                    start_angles.push_back(i);
                }

                break;
            }

        }


        moon = new Moon(offset + scale + moon_size, start_angle, x, y, moon_mass, moon_size, 50.0f);
        moons.push_back(moon);
    }
}



void Planet::Generate_Planets(int num, float scale) {
    if (num < 1)//1 is essentially capture the hill, but doesn't fail game logic
        num = 1;

    if (scale < 0.5)
        scale = 0.5;
    else if (scale > 2.5)
        scale = 2.5;

    int half_num = num / 2;
    float variance = scale * 0.2;

    float mass_modifier = 1000.0;
    float size_modifier = 200.0;
    float interval_modifier = 1000.0;
    float capture_modifier = 0.025;

    Planet * planet = NULL;


    //Create Red Team Planets
    //////////////////////////////////////////////////
    for (int i = half_num; i > 0; i--) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(RED_TEAM, -interval_modifier * scale * i, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier / scale);
        planet->Generate_Moons(size_modifier * scale);
        Planet::planet_graph.push_back(planet);
    }
    //////////////////////////////////////////////////


    //Create Neutral Planet, if number of planets is odd
    //////////////////////////////////////////////////
    if (num % 2 == 1) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(NEUTRAL_TEAM, 0.0, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier / scale);
        planet->Generate_Moons(size_modifier * scale);
        planet->Lock(false);
        Planet::planet_graph.push_back(planet);
    }
    //////////////////////////////////////////////////


    //Create Blue Team Planets
    //////////////////////////////////////////////////
    for (int i = 1; i <= half_num; i++) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(BLUE_TEAM, interval_modifier * scale * i, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier / scale);
        planet->Generate_Moons(size_modifier * scale);
        Planet::planet_graph.push_back(planet);
    }
    //////////////////////////////////////////////////


    //If number of planets is even, unlock the PAIR of middle planets
    //////////////////////////////////////////////////
    if (num % 2 == 0) {
        int i = 0;

        for (std::list<Planet *>::iterator it = planet_graph.begin(); it != planet_graph.end(); ++it) {
            if (i == half_num - 1)
                (*it)->Lock(false);
            else if (i == half_num) {
                (*it)->Lock(false);
                break;
            }

            i++;
        }
    }
    //////////////////////////////////////////////////
}

void Planet::Preview_Planets(int num, float scale, int offsetx, int offsety, float widthScale) {
    if (num < 1)//1 is essentially capture the hill, but doesn't fail game logic
        num = 1;

    if (scale < 0.5)
        scale = 0.5;
    else if (scale > 2.5)
        scale = 2.5;

    int half_num = num / 2;
    float variance = scale * 0.2;

    float mass_modifier = 1000.0;
    float size_modifier = 200.0;
    float interval_modifier = 1000.0;
    float capture_modifier = 0.025;

    Planet * planet = NULL;


    //Create Red Team Planets
    //////////////////////////////////////////////////
    for (int i = half_num; i > 0; i--) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        surface_manager->red_planet->DrawCentered(offsetx + -interval_modifier * scale * i * widthScale, offsety, 0, size_modifier * scale * .2);
        // planet->Generate_Moons(size_modifier * scale);
    }
    //////////////////////////////////////////////////


    //Create Neutral Planet, if number of planets is odd
    //////////////////////////////////////////////////
    if (num % 2 == 1) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        surface_manager->neutral_planet->DrawCentered(offsetx, offsety, 0, size_modifier * scale * .2);
        // planet->Generate_Moons(size_modifier * scale);
    }
    //////////////////////////////////////////////////


    //Create Blue Team Planets
    //////////////////////////////////////////////////
    for (int i = 1; i <= half_num; i++) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        surface_manager->blue_planet->DrawCentered(offsetx + interval_modifier * scale * i * widthScale, offsety, 0, size_modifier * scale * .2);
        // planet->Generate_Moons(size_modifier * scale);
    }
    //////////////////////////////////////////////////
}

void Planet::Clear_Planets() {
    while (!Planet::planet_graph.empty()) {
        delete Planet::planet_graph.front();
        Planet::planet_graph.pop_front();
    }
}

Team Planet::Win_Condition() {
    if (Planet::planet_graph.front()->team_id == BLUE_TEAM)
        return BLUE_TEAM;

    if (Planet::planet_graph.back()->team_id == RED_TEAM)
        return RED_TEAM;

    return NO_TEAM;
}