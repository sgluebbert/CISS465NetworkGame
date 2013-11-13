#include "Entity.h"



Entity::Entity() {
<<<<<<< HEAD
    angle = x = y = w = h = 0.0;
    dx = dy = 0.0;
    velocity = max_velocity = 0.0;
    reverse_modifier = 0.0;
    mass = force = inertia = 0.0;
    turn_rate = 0.0;
}

Entity::Entity(float _x, float _y, float _w, float _h, float _angle, float _mass, float _velocity, float _r)
    : x(_x), y(_y), w(_w), h(_h), angle(_angle), mass(_mass), velocity(_velocity)
{
    bounding_volume.x = _x;
    bounding_volume.y = _y;
    bounding_volume.r = _r;
}


Entity::~Entity() {
}



NetString *Entity::Serialize() {
    NetString * string = new NetString();

    if (!string->WriteFloat(x) || !string->WriteFloat(y) || !string->WriteFloat(angle)) {
        delete string;
        return NULL;
    }

    return string;
}

bool Entity::Deserialize(NetString *string) {
    if (!string->ReadFloat(x) || !string->ReadFloat(y) || !string->ReadFloat(angle))
        return false;

    return true;
}

=======
    //Drawable Init
    draw_scale = draw_angle = 0.0;
>>>>>>> f880b59b79fb4ff98d4747192ddee7a233ae956f

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