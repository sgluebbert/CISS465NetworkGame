#ifndef BULLET_H
#define BULLET_H

#include "System.h"

struct Bullet
{
	Bullet(int, double, double, double, double);
	double x, y, velocity, angle, dx, dy;
	void CalculateSpeed(double);
	bool Move(double);
    int team;
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

#endif
