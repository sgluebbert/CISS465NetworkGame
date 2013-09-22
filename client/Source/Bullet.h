#ifndef BULLET_H
#define BULLET_H

#include <deque>
#include "Camera.h"

struct Bullet
{
	Bullet(double, double, double, double);
	double x, y, velocity, angle, dx, dy;
	void CalculateSpeed(double);
	bool Move(double);
	void Draw();
    void SetSurface(SDL_Surface *, double, double);
private:
    SDL_Surface * surface;
    int width, height;
};

struct Bullet_List
{
	static Bullet_List* getInstance();

	void Update();
	void Draw();
	void AddBullet(double, double, double, double);
private:
	Bullet_List();
	std::deque<Bullet *> bullets;

	static bool instanceFlag;
	static Bullet_List *single;
};

Bullet::Bullet(double _x, double _y, double _velocity, double _angle)
	: x(_x), y(_y), velocity(_velocity), angle(_angle), dx(0), dy(0)
{}

void Bullet::CalculateSpeed(double delta) {
    dx = velocity * TRIG_TABLE[int(angle / 5.0)][1];
    dy = velocity * TRIG_TABLE[int(angle / 5.0)][0];
}

bool Bullet::Move(double delta) {
    x += dx * delta;
    y -= dy * delta;
    if (x < 0 || y < 0 || x > ROOM_WIDTH || y > ROOM_HEIGHT)
    	return true; // die
    return false;
}

void Bullet::Draw() {
    SDL_Rect viewport = Camera::getInstance()->Get_Viewport();

    if (x > viewport.x - width && x < viewport.x + viewport.w + width && y > viewport.y - height && y < viewport.y + viewport.h + height)
	   Surface::Blit(WINDOW, surface, x - width / 2 - viewport.x, y - height / 2 - viewport.y, 0, 0, width, height);
}

void Bullet::SetSurface(SDL_Surface * SurfSrc, double new_width, double new_height) {
    surface = SurfSrc;
    width = new_width;
    height = new_height;
}

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

void Bullet_List::Update()
{
    double delta = GetTimePerFrame();

    for (int i = 0; i < bullets.size(); i++)
    {
    	Bullet *bullet = bullets[i];
    	if (bullet == NULL)
    		continue;

    	bullet->CalculateSpeed(delta);
    	if (bullet->Move(delta))
    	{
    		bullets[i] = NULL;
    		continue;
    	}
    }

    if (bullets.size() > 0 && bullets.front() == NULL)
    	bullets.pop_front();
    if (bullets.size() > 0 && bullets.back() == NULL)
    	bullets.pop_back();
}

void Bullet_List::Draw()
{
    for (int i = 0; i < bullets.size(); i++)
    {
    	Bullet *bullet = bullets[i];
    	if (bullet == NULL)
    		continue;

    	bullet->Draw();
    }
}

void Bullet_List::AddBullet(double x, double y, double velocity, double angle)
{
	Bullet *bullet = new Bullet(x, y, velocity, angle);
	bullet->SetSurface(SurfaceManager::getInstance()->bullet01, 4, 4);
	bullets.push_back(bullet);
}

#endif