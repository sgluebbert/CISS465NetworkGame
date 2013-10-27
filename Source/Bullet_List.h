#ifndef BULLET_LIST_H
#define BULLET_LIST_H



#include <deque>

#include "Bullet.h"
#include "Timer.h"


struct Bullet_List {
    static Bullet_List* getInstance();

    void Update();
    void AddBullet(int, double, double, double, double);

//private:
    Bullet_List();
    std::deque<Bullet *> bullets;

    static bool instanceFlag;
    static Bullet_List *single;
};



bool Bullet_List::instanceFlag = false;
Bullet_List* Bullet_List::single = NULL;



Bullet_List::Bullet_List()
{}

Bullet_List* Bullet_List::getInstance() {
    if (!instanceFlag) {
        single = new Bullet_List();
        instanceFlag = true;
        return single;
    }
    else
        return single;
}

void Bullet_List::Update() {
    double delta = Timer::Frame_Control.Get_Time_Per_Frame();

    for (int i = 0; i < bullets.size(); i++) {
        Bullet *bullet = bullets[i];
        if (bullet == NULL)
            continue;

        bullet->CalculateSpeed(delta);
        if (bullet->Move(delta)) {
            bullets[i] = NULL;
            continue;
        }
    }

    if (bullets.size() > 0 && bullets.front() == NULL)
        bullets.pop_front();
    if (bullets.size() > 0 && bullets.back() == NULL)
        bullets.pop_back();
}

void Bullet_List::AddBullet(int team, double x, double y, double velocity, double angle) {
    Bullet * bullet = new Bullet(team, x, y, velocity, angle);
    bullet->SetTexture(surface_manager->bullet, 8, 8);
    bullets.push_back(bullet);
}



#endif
