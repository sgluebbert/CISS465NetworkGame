#include "Entity.h"



Entity::Entity() {
    //Drawable Init
    draw_scale = draw_angle = 0.0;

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
        texture->DrawCentered(x, y, -draw_angle, draw_scale);
    }
}