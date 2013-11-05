#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Bullet_List.h"
#include "System.h"
#include <ctime>
#include <deque>
#include "Clock.h"
#include "Networking/Parser.h"

const int MAX_BULLETS = 5;

const float spawn_points[][2] = { {60, 60}, {ROOM_WIDTH - 60, ROOM_HEIGHT - 60}, {ROOM_WIDTH / 2, ROOM_HEIGHT / 2}, {ROOM_WIDTH / 3, 60}, {ROOM_WIDTH * .75, 60},
                                  {60, ROOM_HEIGHT / 3}, {60, ROOM_HEIGHT * .75}, {ROOM_WIDTH / 3, ROOM_WIDTH - 60}, {ROOM_WIDTH * .75, ROOM_WIDTH - 60},
                                  {ROOM_HEIGHT - 60, ROOM_HEIGHT / 3}, {ROOM_HEIGHT - 60, ROOM_HEIGHT * .75} };

class Entity;
int GetSpawnPoint(std::deque<Entity*> &entities);

class Entity {
public:
    Entity(int);
    ~Entity();
    
    void TurnLeft(float);
    void TurnRight(float);
    void CalculateSpeed(float);
    void CalculateVelocity(float);
    void Move(float);
    void TryFire();
    void Respawn(std::deque<Entity*> &);
    NetString *Serialize();
    
    virtual void Update(std::deque<Entity*> &);
    
//protected:
    time_t death_time;
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
    float health, max_health;

    float respawn_timer;
    static float respawn_time;

    int can_shoot;
    int used_bullets;
};



float Entity::respawn_time = 5.0;



Entity::Entity(int _id)
    : id(_id) {
    death_time = 0;
    x = 0.0;
    y = 0.0;
    dx = 0.0;
    dy = 0.0;
    width = 64;
    height = 64;
    angle = 0.0;
    turn_rate = 28;
    throttle = 4;
    velocity = 0.0;
    acceleration = 18;
    deceleration = 10;
	max_velocity = 50;
    move_forward = turn_left = turn_right = shoot = did_shoot = false;
    health = max_health = 100;
    can_shoot = 0;
    used_bullets = 0;
    respawn_timer = respawn_time;
}

Entity::~Entity() {
}

void Entity::Respawn(std::deque<Entity*> &entities)
{
    int spawn_point = GetSpawnPoint(entities);
    x = spawn_points[spawn_point][0];
    y = spawn_points[spawn_point][1];
    health = 100;
    velocity = 0;
    can_shoot = 0;
    death_time = 0;
}

NetString *Entity::Serialize()
{
    NetString *string = new NetString();
    string->WriteUChar(id);
    string->WriteFloat(x);
    string->WriteFloat(y);
    string->WriteFloat(angle);
    string->WriteFloat(health);
    string->WriteBool(did_shoot);
    return string;
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

void Entity::Update(std::deque<Entity*> &entities) {
    double delta = Clock::Frame_Control.Get_Time_Per_Frame();
    
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

    if (health == 0)
    {
        if (death_time == 0)
            time(&death_time);
        else
        {
            time_t now;
            time(&now);
            if (now - death_time >= respawn_timer)
                Respawn(entities);
        }
    }
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
}

int GetSpawnPoint(std::deque<Entity*> &entities)
{
    int spawn_points_size = (sizeof(spawn_points) / 8);
    for (int i = 0; i < spawn_points_size; i++)
    {
        bool empty = true;

        for (int n = 0; n < entities.size(); n++)
        {
            if (entities[n] == NULL)
                continue;
            if (point_distance(spawn_points[i][0], spawn_points[i][1], entities[n]->x, entities[n]->y) < 200)
            {
                empty = false;
                break;
            }
        }

        if (empty)
            return i;
    }

    return rand() % spawn_points_size;
}

#endif
