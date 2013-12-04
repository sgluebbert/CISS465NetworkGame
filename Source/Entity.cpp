#include "Entity.h"

std::ostream &operator<<(std::ostream &stream, Team e)
{
    switch (e)
    {
        case NO_TEAM:
            stream << "No Team.";
            break;
        case RED_TEAM:
            stream << "Red.";
            break;
        case BLUE_TEAM:
            stream << "Blue.";
            break;
        case NEUTRAL_TEAM:
            stream << "Neutral.";
            break;
        default:
            stream << "Unknown";
    }
    
    return stream;
}

Entity::Entity() {
    //RigidBody Init
    dx = dy = force = torque = velocity = rotation = 0.0;
    mass = inertia = 1.0;
    x = y = angle = 0.0;
}

void Entity::Update(double dt) {
    velocity *= FRICTION_COEFFICIENT;

    Calculate_Velocity(dt);
    Calculate_Rotation(dt);
    Calculate_Vector(dt);
    Move(dt);

    drawing_box.Update(dx, dy);
}

void Entity::Draw() {
    if (texture == NULL)
        DrawRect(drawing_box.x, drawing_box.y, drawing_box.w, drawing_box.h, false, &WHITE);
    else {
        glColor4f(1.0, 1.0, 1.0, 1.0);
        texture->DrawCentered(drawing_box.x + drawing_box.w / 2.0, drawing_box.y + drawing_box.h / 2.0, -draw_angle, draw_scale);
    }
}

void Entity::Sync() {
    drawing_box.x = x - draw_scale / 2.0;
    drawing_box.y = y - draw_scale / 2.0;
}