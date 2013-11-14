#include "Planet.h"


Planet::Planet(int _id, PlanetState s, float _x, float _y, float _a, float m, float _v, float _r, float gr, float cv)
    : id(_id), state(s), r(_r), gravity_radius(gr), capture_value(cv)
{
    Collidable::objects.push_back(this);
    Drawable::objects.push_back(this);
    Rigid_Body::objects.push_back(this);
    
    texture = NULL;
    x = _x;
    y =  _y;
    angle = _a;
    mass =  m;
    velocity = _v;

    bounding_volume.r = _r + gr;
    bounding_volume.x = x;
    bounding_volume.y = y;
}

Planet::~Planet()
{

}

void Planet::SetTexture(Texture * tex) 
{
    texture = tex;
}


void Planet::UnderSiege(Ship ship)
{
    bounding_volume.r = r + gravity_radius;
    bounding_volume.x = x;
    bounding_volume.y = y;

    if (DoCollide(this, &ship))
    {
        if (ship.team_id == 1)
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
            state = BLUE_PLANET;
            SetTexture(surface_manager->blue_planet);
        }

        if (capture_value < -1.0f)
        {
            capture_value = -1.0f;
            state = RED_PLANET;
            SetTexture(surface_manager->red_planet);
        }

        if (state == BLUE_PLANET && capture_value < 0.0f)
        {
            state = NEUTRAL;
        }

        if (state == RED_PLANET && capture_value > 0.0f)
        {
            state = NEUTRAL;
        }

        if (capture_value == 0.0f || state == NEUTRAL)
        {
            state = NEUTRAL;
            SetTexture(surface_manager->neutral_planet);
        }
    }
}

void Planet::DrawGravityField()
{
    if (capture_value > 0.0f)
    {
        // light blue color
        const Color blue(0.5f, 0.5f, 0.5f + capture_value/2.0f, 0.3f);
        DrawCircle(x, y, r + gravity_radius, 1, &blue);
    }
    else if (capture_value < 0.0f)
    {
        // light red color
        const Color red(0.5f + -capture_value/2.0f, 0.5f, 0.5f, 0.3f);
        DrawCircle(x, y, r + gravity_radius, 1, &red);
    }
    else
    {
        // light gray color
        const Color gray(0.5f, 0.5f, 0.5f, 0.3f);
        DrawCircle(x, y, r + gravity_radius, 1, &gray);
    }

}

void Planet::Update(double dt)
{
    Entity::Update(dt);
}

void Planet::Draw() 
{
    if (texture == NULL)
        return;

    DrawGravityField();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    int w = 10;// ?
    texture->DrawCentered(x, y, -angle, r);
}


PlanetsHUD::PlanetsHUD(int num)
    : num_planets(num)
{
    int x = 20;
    int y = 150;
    int w = 100;
    int h = 10;
    int b = 0;

    Color lhs(1.0f, 0.0f, 0.0f, 1.0f);
    Color rhs(0.0f, 0.0f, 1.0f, 1.0f);
    Color back_color(0.5f, 0.5f, 0.5f, 1.0f);

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