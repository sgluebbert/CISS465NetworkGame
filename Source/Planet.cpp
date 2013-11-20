#include "Planet.h"



std::list<Planet *> Planet::planet_graph;



Planet::Planet(Team _id, float _x, float _y, float _a, float m, float _v, float _r, float gr, float cv)
    : gravity_radius(gr), capture_value(cv)
{
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    planet_graph.push_back(this);

    Set_Group(PLANET_GROUP);

    team_id = _id;

    switch (team_id) {
        case RED_TEAM:
            locked = true;
            texture = surface_manager->red_planet;
            break;
        case BLUE_TEAM:
            locked = true;
            texture = surface_manager->blue_planet;
            break;
        default:
            locked = false;
            texture = surface_manager->neutral_planet;
            break;
    }

    dx = dy = force = torque = rotation = 0.0;
    
    field = NULL;

    x = bounding_volume.x = _x;
    y = bounding_volume.y = _y;
    draw_angle = angle = _a;
    draw_scale = bounding_volume.r = _r + gr;
    drawing_box.x = x - bounding_volume.r;
    drawing_box.y = y - bounding_volume.r;
    drawing_box.w = 2 * bounding_volume.r;
    drawing_box.h = 2 * bounding_volume.r;

    mass =  m;
    velocity = _v;
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
    if (capture_value > 0.0f)
    {
        // light blue color
        const Color blue(0.5f, 0.5f, 0.5f + capture_value/2.0f, 0.3f);
        DrawCircle(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, draw_scale, 1, &blue);
    }
    else if (capture_value < 0.0f)
    {
        // light red color
        const Color red(0.5f + -capture_value/2.0f, 0.5f, 0.5f, 0.3f);
        DrawCircle(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, draw_scale, 1, &red);
    }
    else
    {
        // light gray color
        const Color gray(0.5f, 0.5f, 0.5f, 0.3f);
        DrawCircle(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, draw_scale, 1, &gray);
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


PlanetsHUD::PlanetsHUD(int num)
    : num_planets(num)
{
    int x = 20;
    int y = 150;
    int w = 100;
    int h = 10;
    int b = 0;

    Color lhs(0.5f, 0.0f, 0.0f, 1.0f);
    Color rhs(0.0f, 0.0f, 0.5f, 1.0f);
    Color back_color(0.25f, 0.25f, 0.25f, 1.0f);

    for (int i = 0; i < num; i++)
    {
        Alignment_Bar * bar = new Alignment_Bar();
        bar->Set_Rect(x, y, w, h, b);
        bar->progress = 0.0f;
        bar->back_color = back_color;
        bar->lhs_color = lhs;
        bar->rhs_color = rhs;

        y += (h + 5);

        bars.push_back(bar);
    }

}

void PlanetsHUD::Update(std::vector<Planet *> planets)
{
    for (int i = 0; i < planets.size(); i++)
    {
        bars[i]->progress = planets[i]->capture_value;
    }

}

void PlanetsHUD::Draw()
{
    for (int i = 0; i < num_planets; i++)
    { 
        std::string string = "P";
        string += int_to_string(i);
        Color color(1.0f, 1.0f, 1.0f, 1.0f);
        Text text(string.c_str(), TextureManager::GetInstance()->fonts.font_FreeMono_10, color);
        text.Draw(bars[i]->border_rect.x - 15, bars[i]->border_rect.y);

        bars[i]->Draw();
    }
}