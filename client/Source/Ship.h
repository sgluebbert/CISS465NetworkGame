#ifndef SHIP_H
#define SHIP_H



#include <SDL.h>

#include "Bullet_List.h"
#include "System.h"
#include "Timer.h"



class Ship {
public:
    Ship(int);
    ~Ship();
    
    bool LoadSurface(const char *, float, float);
    void SetSurface(SDL_Surface *, float, float);

    SDL_Rect Get_Bounding_Box();
    
    void TurnLeft(float);
    void TurnRight(float);
    void CalculateSpeed(float);
    void CalculateVelocity(float);
    void Move(float);
    void TryFire();

    void Take_Damage(float);
    float Get_Health();
    
    virtual void Draw();
    virtual void Update();
    
//protected:
    SDL_Surface * Ship_surface;
    
    int team;
    float x, y;
    float dx, dy;
    float width, height;
    
    float turn_rate;
    float angle;
    
    float throttle;
    float max_velocity;
    float velocity;
    float acceleration;
    float deceleration;
    bool move_forward, turn_left, turn_right, shoot;
    int can_shoot, reload_rate;
    float health, max_health;
};



Ship::Ship(int _team) 
    : team(_team) {
    Ship_surface = NULL;
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

Ship::~Ship() {
}

void Ship::SetSurface(SDL_Surface * SurfSrc, float new_width, float new_height) {
    Ship_surface = SurfSrc;
    width = new_width;
    height = new_height;
}

SDL_Rect Ship::Get_Bounding_Box() {
    SDL_Rect temp;

    temp.x = x;
    temp.y = y;
    temp.w = width;
    temp.h = height;

    return temp;
}

void Ship::Draw() {
    if (Ship_surface == NULL)
        return;

    int index = round(angle / (360 / 72));
	if (index >= 72) index = 71;
    if (index < 0) index = 0;

	Surface_Manager::Blit(WINDOW, Ship_surface, x - width / 2, y - height / 2, index * width, 0, width, height);
}

void Ship::CalculateSpeed(float delta) {
    // dx = velocity * TRIG_TABLE[int(angle / 5.0)][1];
    // dy = velocity * TRIG_TABLE[int(angle / 5.0)][0];
}

void Ship::CalculateVelocity(float delta) {
    // float tempA = acceleration * delta;
    // float tempD = deceleration * delta;
    
    // if (velocity > max_velocity * throttle) {
    //     velocity -= tempD;
        
    //     if (velocity < max_velocity * throttle)
    //         velocity = max_velocity * throttle;
    // }
    // else if (velocity < max_velocity * throttle) {
    //     velocity += tempA;
        
    //     if (velocity > max_velocity * throttle)
    //         velocity = max_velocity * throttle;
    // }
}

void Ship::TurnLeft(float delta) {
    // angle += turn_rate * delta;
    // if (angle >= 360) angle = angle - 360;
}

void Ship::TurnRight(float delta) {
    // angle -= turn_rate * delta;
    // if (angle < 0) angle = 360 + angle;
}

void Ship::Move(float delta) {
    // x += dx * delta;
    // y -= dy * delta;
}

void Ship::Update() {
    // float delta = Timer::Frame_Control.Get_Time_Per_Frame();
    
    // if (move_forward)
    //     throttle = 1;
    // else
    //     throttle = 0;

    // if (turn_left)
    //     TurnLeft(delta);
    // if (turn_right)
    //     TurnRight(delta);

    // CalculateVelocity(delta);
    // CalculateSpeed(delta);
    // Move(delta);

    Bullet_List *bullet_list = Bullet_List::getInstance();
    for (int i = 0; i < bullet_list->bullets.size(); i++)
    {
        Bullet *bullet = bullet_list->bullets[i];
        if (bullet == NULL || bullet->team == team)
            continue;

        if (point_in_rect(bullet->x, bullet->y, x - width / 3, y - height / 3, x + width / 3, y + height / 3))
        {
            delete bullet;
            bullet_list->bullets[i] = NULL;
        }
    }

    // if (can_shoot > 0)
    // 	can_shoot--;
    // if (shoot)
    // 	TryFire();
}

void Ship::TryFire()
{
	// if (can_shoot == 0) {
	//     //Bullet_List * bullet_list = Bullet_List::getInstance();

	//     int offset_x = 20 * TRIG_TABLE[int(angle / 5.0)][1];
 //    	    int offset_y = 20 * TRIG_TABLE[int(angle / 5.0)][0];
	//     //bullet_list->AddBullet(x + offset_x, y - offset_y, velocity + 100, angle);
	//     can_shoot = reload_rate;
	// }
}

void Ship::Take_Damage(float damage) {
    // health -= damage;
}

float Ship::Get_Health() {
    return health / max_health;
}



#endif
