#include "Planet.h"



std::list<Planet *> Planet::planet_graph;
float Planet::field_modifier = 1.0;



Planet::Planet(Team _id, float _x, float _y, float _m, float _r, float _cr)
    : gravity_radius(_r * field_modifier), capture_rate(_cr)
{
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);

    Set_Group(PLANET_GROUP);

    team_id = _id;

    switch (team_id) {
        case RED_TEAM:
            locked = true;
            alignment = -1.0;
            texture = surface_manager->red_planet;
            break;
        case BLUE_TEAM:
            locked = true;
            alignment = 1.0;
            texture = surface_manager->blue_planet;
            break;
        default:
            locked = false;
            alignment = 0.0;
            texture = surface_manager->neutral_planet;
            break;
    }

    dx = dy = force = torque =velocity = rotation = 0.0;
    
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


void Planet::UnderSiege(Ship * ship)
{
    if (locked)
        return;//Skip collision testing for gravity field

    if (DoCollide(this, ship))
    {
        if (ship->team_id == RED_TEAM)
        {
            alignment -= capture_rate * ship->capture_modifier;
        }
        else
        {
            alignment += capture_rate * ship->capture_modifier;
        }


        if (alignment > 1.0f)
        {
            alignment = 1.0f;
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

        if (alignment < -1.0f)
        {
            alignment = -1.0f;
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

        if (team_id == BLUE_TEAM && alignment < 0.0f)
        {
            team_id = NEUTRAL_TEAM;

            /*Lock the left neighbor*/
            //////////////////////////////////////////////////
            Planet * neighbor = NULL;

            for (std::list<Planet *>::reverse_iterator it = planet_graph.rbegin(); it != planet_graph.rend(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.rend())
                        neighbor = *(++it);

            if (neighbor != NULL) {
                neighbor->locked = true;
                neighbor->alignment = -1.0;
            }
            //////////////////////////////////////////////////
        }

        if (team_id == RED_TEAM && alignment > 0.0f)
        {
            team_id = NEUTRAL_TEAM;

            /*Lock and reset the right neighbor*/
            //////////////////////////////////////////////////
            Planet * neighbor = NULL;

            for (std::list<Planet *>::iterator it = planet_graph.begin(); it != planet_graph.end(); ++it)
                if (*it == this)
                    if ((++it)-- != planet_graph.end())
                        neighbor = *(++it);

            if (neighbor != NULL) {
                neighbor->locked = true;
                neighbor->alignment = 1.0;
            }
            //////////////////////////////////////////////////
        }

        if (alignment == 0.0f || team_id == NEUTRAL_TEAM)
        {
            texture = surface_manager->neutral_planet;
        }
    }
}

void Planet::DrawGravityField()
{
    if (field == NULL)
        return;

    if (alignment > 0.0f)
    {
        // light blue color
        glColor4f(0.5f, 0.5f, 0.5f + alignment/2.0f, 0.3f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
    else if (alignment < 0.0f)
    {
        // light red color
        glColor4f(0.5f + -alignment/2.0f, 0.5f, 0.5f, 0.3f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
    else
    {
        // light gray color
        glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
        field->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }

}

void Planet::Update(double dt)
{
    Entity::Update(dt);
    drawing_box.Update(dx, -dy);
    bounding_volume.Update(dx, -dy);
}

void Planet::Draw() 
{
    if (texture == NULL)
        return;

    DrawGravityField();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale - gravity_radius);
}



void Planet::Generate_Planets(int num, float scale) {
    if (num < 2)//2 is the minimum number of planets
        num = 2;

    if (scale < 0.5)
        scale = 0.5;
    else if (scale > 2.5)
        scale = 2.5;

    int half_num = num / 2;
    float variance = scale * 0.2;

    float mass_modifier = 1000.0;
    float size_modifier = 200.0;
    float interval_modifier = 1000.0;
    float capture_modifier = 0.001;

    Planet * planet = NULL;

    for (int i = half_num; i > 0; i--) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(RED_TEAM, -interval_modifier * scale * i, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier * scale);
        Planet::planet_graph.push_back(planet);
    }

    if (num % 2 == 1) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(NEUTRAL_TEAM, 0.0, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier * scale);
        Planet::planet_graph.push_back(planet);
    }

    for (int i = 1; i <= half_num; i++) {
        scale = (float(rand()) / float(RAND_MAX) * 2 * variance) - variance + scale;
        planet = new Planet(BLUE_TEAM, interval_modifier * scale * i, 0.0, mass_modifier * scale, size_modifier * scale, capture_modifier * scale);
        Planet::planet_graph.push_back(planet);
    }

    if (num & 2 == 0) {
        int i = 0;

        for (std::list<Planet *>::iterator it = planet_graph.begin(); it != planet_graph.end(); ++it) {
            if (i == half_num)
                (*it)->locked = false;
            else if (i == half_num + 1) {
                (*it)->locked = false;
                break;
            }

            i++;
        }
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