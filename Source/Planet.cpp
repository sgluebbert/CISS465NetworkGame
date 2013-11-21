#include "Planet.h"



std::list<Planet *> Planet::planet_graph;
float Planet::field_modifier = 1.0;



Planet::Planet(Team _id, float _x, float _y, float _m, float _r)
    : gravity_radius(_r * field_modifier)
{
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);

    Set_Group(PLANET_GROUP);

    team_id = _id;

    switch (team_id) {
        case RED_TEAM:
            locked = true;
            capture_value = -1.0;
            texture = surface_manager->red_planet;
            break;
        case BLUE_TEAM:
            locked = true;
            capture_value = 1.0;
            texture = surface_manager->blue_planet;
            break;
        default:
            locked = false;
            capture_value = 0.0;
            texture = surface_manager->neutral_planet;
            break;
    }

    field = NULL;

    dx = dy = force = torque =velocity = rotation = 0.0;
    
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


void Planet::UnderSiege(Ship * ship)
{
    if (locked)
        return;//Skip collision testing for gravity field

    if (DoCollide(this, ship))
    {
        if (ship->team_id == RED_TEAM)
        {
            capture_value -= 0.001f;
        }
        else
        {
            capture_value += 0.001f;
        }


        if (capture_value > 1.0f)
        {
            capture_value = 1.0f;
            team_id = BLUE_TEAM;
            texture = surface_manager->blue_planet;

            /*Lock the right neighbor*/
            //////////////////////////////////////////////////
            Planet * neighbor = NULL;

            for (std::list<Planet *>::iterator it = planet_graph.begin(); it != planet_graph.end(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.end())
                        neighbor = *(++it);

            if (neighbor != NULL)
                neighbor->locked = true;
            //////////////////////////////////////////////////

            /*Unlock the left neighbor*/
            //////////////////////////////////////////////////
            neighbor = NULL;

            for (std::list<Planet *>::reverse_iterator it = planet_graph.rbegin(); it != planet_graph.rend(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.rend())
                        neighbor = *(++it);

            if (neighbor != NULL)
                neighbor->locked = false;
            //////////////////////////////////////////////////
        }

        if (capture_value < -1.0f)
        {
            capture_value = -1.0f;
            team_id = RED_TEAM;
            texture = surface_manager->red_planet;

            /*Lock the left neighbor*/
            //////////////////////////////////////////////////
            Planet * neighbor = NULL;

            for (std::list<Planet *>::reverse_iterator it = planet_graph.rbegin(); it != planet_graph.rend(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.rend())
                        neighbor = *(++it);

            if (neighbor != NULL)
                neighbor->locked = true;
            //////////////////////////////////////////////////

            /*Unlock the right neighbor*/
            //////////////////////////////////////////////////
            neighbor = NULL;

            for (std::list<Planet *>::iterator it = planet_graph.begin(); it != planet_graph.end(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.end())
                        neighbor = *(it);

            if (neighbor != NULL)
                neighbor->locked = false;
            //////////////////////////////////////////////////
        }

        if (team_id == BLUE_TEAM && capture_value < 0.0f)
        {
            team_id = NEUTRAL_TEAM;
        }

        if (team_id == RED_TEAM && capture_value > 0.0f)
        {
            team_id = NEUTRAL_TEAM;
        }

        if (capture_value == 0.0f || team_id == NEUTRAL_TEAM)
        {
            texture = surface_manager->neutral_planet;
        }
    }
}

void Planet::DrawGravityField()
{

    field = surface_manager->field_glow_unlocked;

    if (locked)
        field = surface_manager->field_glow_locked;

    if (capture_value > 0.0f)
    {
        glColor4f(0.5f, 0.5f, 0.5f + capture_value/2.0f, 0.5f);
    }
    else if (capture_value < 0.0f)
    {
        glColor4f(0.5f + -capture_value/2.0f, 0.5f, 0.5f, 0.5f);
    }
    else
    {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    }

    field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
}

void Planet::Update(double dt)
{
    Entity::Update(dt);
    drawing_box.Update(dx, -dy);
    bounding_volume.Update(dx, -dy);

    for (int i = 0; i < moons.size();i++)
    {
        moons[i]->Update(dt);
        moons[i]->team_id = team_id;
    }

}

void Planet::Draw() 
{
    if (texture == NULL)
        return;

    DrawGravityField();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale - gravity_radius);

    for (int i = 0; i < moons.size(); i++)
    {
        moons[i]->Draw();
    }

}

void Planet::Generate_Moons()
{
    int num_moons = -1;
    float num_moon_calc = mass;

    while (num_moon_calc > 1)
    {
        num_moon_calc /= 10;
        num_moons++;
    }

    if (num_moons == -1)
        num_moons = 2;

    float moon_size = mass / (float)num_moons;

    Moon * moon = NULL;
    for (int i = 0; i < num_moons; i++)
    {
        moon = new Moon(team_id, x, y + 1000, moon_size, 100.0f, 50.0f);
        moons.push_back(moon);
    }
}


void Planet::Generate_Planets(int num) {
    if (num < 2)//2 is the minimum number of planets
        num = 2;

    int half_num = num / 2;
    float size = 200.0;
    float mass = 1000.0;
    float alignment;
    Planet * planet = NULL;

    for (int i = half_num; i > 0; i--) {
        planet = new Planet(RED_TEAM, -1000.0 * i, 0.0, mass, size);
        planet->Generate_Moons();
        Planet::planet_graph.push_back(planet);
    }

    if (num % 2 == 1) {
        planet = new Planet(NEUTRAL_TEAM, 0.0, 0.0, mass, size);
        planet->Generate_Moons();
        Planet::planet_graph.push_back(planet);
    }

    for (int i = 1; i <= half_num; i++) {
        planet = new Planet(BLUE_TEAM, 1000.0 * i, 0.0, mass, size);
        planet->Generate_Moons();
        Planet::planet_graph.push_back(planet);
    }
}

void Planet::Clear_Planets() {
    while (!Planet::planet_graph.empty()) {
        delete Planet::planet_graph.front();
        Planet::planet_graph.pop_front();
    }

    Planet::planet_graph.clear();
}

Team Planet::Win_Condition() {
    if (Planet::planet_graph.front()->team_id == BLUE_TEAM)
        return BLUE_TEAM;

    if (Planet::planet_graph.back()->team_id == RED_TEAM)
        return RED_TEAM;

    return NO_TEAM;
}