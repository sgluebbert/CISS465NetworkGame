#ifndef BULLET_H
#define BULLET_H



#include "Timer.h"



struct Bullet {
    double x, y, velocity, angle, dx, dy;

	Bullet(int, double, double, double, double);

	void CalculateSpeed(double);
	bool Move(double);
	void Draw();
    void SetSurface(SDL_Surface *, double, double);
    SDL_Rect Get_Bounding_Box();

    int team;
private:
    SDL_Surface * surface;
    int width, height;
};



Bullet::Bullet(int _team, double _x, double _y, double _velocity, double _angle)
	: x(_x), y(_y), velocity(_velocity), angle(_angle), dx(0), dy(0), team(_team)
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
    // int index = round(angle / (360 / 72));
    // if (index >= 72) index = 71;
    // if (index < 0) index = 0;

    // Surface_Manager::Blit(WINDOW, surface, x, y, index * width, 0, width, height);
    Surface_Manager::Blit(WINDOW, surface, x, y, 0, 0, width, height);
}

void Bullet::SetSurface(SDL_Surface * SurfSrc, double new_width, double new_height) {
    surface = SurfSrc;
    width = new_width;
    height = new_height;
}

SDL_Rect Bullet::Get_Bounding_Box() {
    SDL_Rect temp;
    
    temp.x = x;
    temp.y = y;
    temp.w = width;
    temp.h = height;

    return temp;
}



#endif