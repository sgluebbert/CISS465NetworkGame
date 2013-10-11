#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Bullet_List.h"
#include "System.h"
#include <ctime>
#include <deque>

const int MAX_BULLETS = 5;

class Entity {
public:
    Entity(Uint64, Uint32, int);
    ~Entity();
    
    void TurnLeft(float);
    void TurnRight(float);
    void CalculateSpeed(float);
    void CalculateVelocity(float);
    void Move(float);
    void TryFire();
    
    //virtual void Events(SDL_Event *);
    virtual void Update();
    
//protected:
    Uint64 ip;
    Uint32 port;
    clock_t last_input;
    int id;
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
    bool move_forward, turn_left, turn_right, shoot, did_shoot;
    float health;

    int can_shoot;
    int used_bullets;
    // Bullet* bullets[MAX_BULLETS];
};

Entity::Entity(Uint64 _ip, Uint32 _port, int _id)
    : ip(_ip), port(_port), id(_id) {
    x = 0.0;
    y = 0.0;
    dx = 0.0;
    dy = 0.0;
    width = 64;
    height = 64;
    angle = 0.0;
    turn_rate = 24;
    throttle = 4;
    velocity = 0.0;
    acceleration = 14;
    deceleration = 10;
	max_velocity = 40;
    move_forward = turn_left = turn_right = shoot = did_shoot = false;
    health = 100;
    can_shoot = 0;
    used_bullets = 0;
    // for (int i = 0; i < MAX_BULLETS; ++i) {
    // 	bullets[i] = NULL;
    // }
}

Entity::~Entity() {
    // for (int i = 0; i < MAX_BULLETS; ++i) {
    //     if (bullets[i] != NULL)
    //         delete bullets[i];
    // }
}

void Entity::CalculateSpeed(float delta) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][1];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][0];
}

void Entity::CalculateVelocity(float delta) {
    float tempA = acceleration * delta;
    float tempD = deceleration * delta;
    
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

void Entity::TurnLeft(float delta) {
    angle += turn_rate * delta;
    if (angle >= 360) angle = angle - 360;
}

void Entity::TurnRight(float delta) {
    angle -= turn_rate * delta;
    if (angle < 0) angle = 360 + angle;
}

void Entity::Move(float delta) {
    x += dx * delta;
    y -= dy * delta;
}

void Entity::Update() {
    double delta = Timer::Frame_Control.Get_Time_Per_Frame();
    
    did_shoot = false;

    if (move_forward)
        throttle = 1;
    else
        throttle = 0;

    if (can_shoot > 0)
    	can_shoot--;

    Bullet_List *bullet_list = Bullet_List::getInstance();
    for (int i = 0; i < bullet_list->bullets.size(); i++)
    {
        Bullet *bullet = bullet_list->bullets[i];
        if (bullet == NULL || bullet->team == id)
            continue;

        if (point_in_rect(bullet->x, bullet->y, x - width / 3, y - height / 3, x + width / 3, y + height / 3))
        {
            delete bullet;
            bullet_list->bullets[i] = NULL;
            health -= 10;
        }
    }

    // for (int i = 0; i < MAX_BULLETS; ++i) {
    // 	if (bullets[i] == NULL)
    // 		continue;

    // 	if (bullets[i]->Move(delta)) { // bullet died
    // 		used_bullets--;
    // 		delete bullets[i];
    // 		bullets[i] = NULL;
    // 	}
    // 	else
    // 		bullets[i]->CalculateSpeed(delta);
    // }

    // for (int i = 0; i < entities.size(); i++) {
    //     Entity *entity = entities[i];
    //     if (entity == NULL || entity == this)
    //         continue;

    //     for (int n = 0; n < MAX_BULLETS; n++) {
    //         Bullet *bullet = entity->bullets[i];
    //         if (bullet == NULL)
    //             continue;

    //         if (point_in_rect(bullet->x, bullet->y, x - width / 2, y - height / 2, x + width / 2, y + height / 2))
    //         {
    //             entity->used_bullets -= 1;
    //             collisions.push_back(new Collision(bullet->x, bullet->y));
    //             delete entity->bullets[i];
    //             entity->bullets[i] = NULL;
    //             health -= 10;
    //         }
    //     }
    // }

    if (x < width) x = width;
    if (y < height) y = height;
    if (x > ROOM_WIDTH - width) x = ROOM_WIDTH - width;
    if (y > ROOM_HEIGHT - height) y = ROOM_HEIGHT - height;

    if (turn_left)
        TurnLeft(delta);
    if (turn_right)
        TurnRight(delta);
    if (shoot)
        TryFire();

    CalculateVelocity(delta);
    CalculateSpeed(delta);
    Move(delta);

    if (health < 0)
        health = 0;
}

void Entity::TryFire()
{
	if (can_shoot != 0 || used_bullets >= MAX_BULLETS)
		return;

    int offset_x = 20 * TRIG_TABLE[int(angle / 5.0)][1];
    int offset_y = 20 * TRIG_TABLE[int(angle / 5.0)][0];
    Bullet_List::getInstance()->AddBullet(id, x + offset_x, y + offset_y, velocity + 120, angle);
	can_shoot = 60;
    did_shoot = true;
 //    used_bullets++;
	// for (int i = 0; i < MAX_BULLETS; ++i) {
	// 	if (bullets[i] == NULL) {
 //            int offset_x = 20 * TRIG_TABLE[int(angle / 5.0)][1];
 //            int offset_y = 20 * TRIG_TABLE[int(angle / 5.0)][0];
	// 		bullets[i] = new Bullet(id, x + offset_x, y + offset_y, velocity + 60, angle);
	// 		break;
	// 	}
	// }
    // bullet_list->AddBullet(x + width / 2, y + height / 2, velocity + 60, angle);
}

#endif
