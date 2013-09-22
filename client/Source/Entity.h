#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Camera.h"
#include "FPS.h"
#include "System.h"

class Entity {
public:
    Entity();
    ~Entity();
    
    bool LoadSurface(const char *, double, double);
    void SetSurface(SDL_Surface *, double, double);
    
    void TurnLeft(double);
    void TurnRight(double);
    void CalculateSpeed(double);
    void CalculateVelocity(double);
    void Move(double);
    void TryFire();
    
    //virtual void Events(SDL_Event *);
    virtual void Draw();
    virtual void Update();
    
//protected:
    SDL_Surface * entity_surface;
    
    double x, y;
    double dx, dy;
    double width, height;
    
    double turn_rate;
    double angle;
    
    double throttle;
    double max_velocity;
    double velocity;
    double acceleration;
    double deceleration;
    bool move_forward, turn_left, turn_right, shoot;
    int can_shoot, reload_rate;
    double health, max_health;
};

Entity::Entity() {
    entity_surface = NULL;
    x = 0.0;
    y = 0.0;
    dx = 0.0;
    dy = 0.0;
    width = 0.0;
    height = 0.0;
    angle = 0.0;
    turn_rate = 0.0;
    throttle = 0.0;
    velocity = 0.0;
    acceleration = 0.0;
    deceleration = 0.0;
    move_forward = turn_left = turn_right = shoot = false;
    can_shoot = 0;
    reload_rate = 20;
    health = max_health = 100;
}

Entity::~Entity() {
    SDL_FreeSurface(entity_surface);
}

bool Entity::LoadSurface(const char * File, double new_width, double new_height) {
    entity_surface = Surface::OnLoad(File);
    width = new_width;
    height = new_height;
    
    return entity_surface == NULL;
}

void Entity::SetSurface(SDL_Surface * SurfSrc, double new_width, double new_height) {
    entity_surface = SurfSrc;
    width = new_width;
    height = new_height;
}

void Entity::Draw() {
    if (entity_surface == NULL)
        return;

    SDL_Rect viewport = Camera::getInstance()->Get_Viewport();

    if (x > viewport.x - width && x < viewport.x + viewport.w + width && y > viewport.y - height && y < viewport.y + viewport.h + height)
    {
	    int index = round(angle / (360 / 72));
	    if (index >= 72) index = 71;
	    if (index < 0) index = 0;

	    Surface::Blit(WINDOW, entity_surface, x - width / 2 - viewport.x, y - height / 2 - viewport.y, index * width, 0, width, height);
	}
}

void Entity::CalculateSpeed(double delta) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][1];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][0];
}

void Entity::CalculateVelocity(double delta) {
    double tempA = acceleration * delta;
    double tempD = deceleration * delta;
    
    if (velocity > max_velocity * throttle) {
        velocity -= tempD;
        
        if (velocity < max_velocity * throttle)
            velocity = max_velocity * throttle;
    }
    else if (velocity < max_velocity * throttle) {
        velocity += tempA;
        
        if (velocity > max_velocity * throttle)
            velocity = max_velocity * throttle;
    }
}

void Entity::TurnLeft(double delta) {
    angle += turn_rate * delta;
    if (angle >= 360) angle = angle - 360;
}

void Entity::TurnRight(double delta) {
    angle -= turn_rate * delta;
    if (angle < 0) angle = 360 + angle;
}

void Entity::Move(double delta) {
    x += dx * delta;
    y -= dy * delta;
}

void Entity::Update() {
    double delta = GetTimePerFrame();
    
    if (move_forward)
    {
        throttle = 1;
    }
    else
        throttle = 0;

    if (turn_left)
        TurnLeft(delta);
    if (turn_right)
        TurnRight(delta);

    CalculateVelocity(delta);
    CalculateSpeed(delta);
    Move(delta);

    if (can_shoot > 0)
    	can_shoot--;
    if (shoot)
    	TryFire();
}

void Entity::TryFire()
{
	if (can_shoot == 0)
	{
	    Bullet_List *bullet_list = Bullet_List::getInstance();

	    int offset_x = 20 * TRIG_TABLE[int(angle / 5.0)][1];
    	int offset_y = 20 * TRIG_TABLE[int(angle / 5.0)][0];
	    bullet_list->AddBullet(x + offset_x, y - offset_y, velocity + 100, angle);
	    can_shoot = reload_rate;
	}
}

#endif
