#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Surface.h"
#include "FPS.h"
#include "System.h"

class Entity {
public:
    Entity();
    ~Entity();
    
    bool LoadSurface(const char *, double, double);
    void SetSurface(SDL_Surface *, double, double);
    
    void Turn_Left(double);
    void Turn_Right(double);
    void CalculateSpeed(double);
    void CalculateVelocity(double);
    void Move(double);
    
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
        
    Surface::Blit(WINDOW, entity_surface, x, y, 0, 0, width, height);
}

void Entity::CalculateSpeed(double delta) {
    dx = max_velocity * TRIG_TABLE[int(angle / 5.0)][0];
    dy = max_velocity * TRIG_TABLE[int(angle / 5.0)][1];
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

void Entity::Turn_Left(double delta) {
    angle -= turn_rate * delta;
}

void Entity::Turn_Right(double delta) {
    angle += turn_rate * delta;
}

void Entity::Move(double delta) {
    x += dx * delta;
    y += dy * delta;
}

void Entity::Update() {
    double delta = GetTimePerFrame();
    
    CalculateVelocity(delta);
    CalculateSpeed(delta);
    Move(delta);
}

#endif
