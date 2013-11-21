#ifndef DEBUG_H
#define DEBUG_H



#include <iostream>
#include <iomanip>

#include "Asteroid.h"
#include "Camera.h"
#include "Collision.h"
#include "Moon.h"
#include "Particles/Particle.h"
#include "Planet.h"
#include "Powerup.h"
#include "Ship.h"



template <class T>
std::ostream & operator<<(std::ostream & cout, const Circle<T> & other) {
	cout << "Center: (" << other.x << ", " << other.y << ")\t\tRadius: " << other.r << std::endl;
	return cout;
}

template <class T>
std::ostream & operator<<(std::ostream & cout, const Rect<T> & other) {
	cout << "Pos: (" << other.x << ", " << other.y << ")\t\tSize: (" << other.w << ", " << other.h << ')' << std::endl;
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Collidable * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Drawable * other) {
	cout << "Drawing Box: " << other->drawing_box;
	cout << "Angle: " << other->draw_angle << "\t\t\t\tScale: " << other->draw_scale << std::endl;
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Motor * other) {
	cout << "Velocity Limit: " << other->velocity_limit << "\t\t\tForce: " << other->force_motor << std::endl;
	cout << "Reverse Modifier: " << other->reverse_modifier << std::endl;
	cout << "Rotation Limit: " << other->rotation_limit << "\t\t\tTorque: " << other->torque_motor << std::endl;
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Rigid_Body * other) {
	cout << "Pos: (" << other->x << ", " << other->y << ')' << std::endl;
	cout << "Vector: (" << other->dx << ", " << other->dy << ")\t\t\t\tAngle: " << other->angle << std::endl;
	cout << "Mass: " << other->mass << "\t\t\t\tInertia: " << other->inertia << std::endl;
	cout << "Velocity: " << other->velocity << "\t\t\t\tForce: " << other->force << std::endl;
	cout << "Rotation: " << other->rotation << "\t\t\t\tTorque: " << other->torque << std::endl;
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Serializable * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Asteroid & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Camera & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Collision & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Moon & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Particle & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Planet & other) {
	cout << (Collidable *) &other;
	cout << (Drawable *) &other;
	cout << (Rigid_Body *) &other;
	cout << "Alignment: " << other.alignment << "\t\t\tCapture Rate: " << other.capture_rate << std::endl;
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Powerup & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Ship & other) {
	cout << (Collidable *) &other;
	cout << (Drawable *) &other;
	cout << (Rigid_Body *) &other;
	cout << (Motor *) &other;
	cout << "Hull: [" << other.health << '/'<< other.max_health << "]\t\t\t\tShields: [" << other.shields << '/'<< other.max_shields << ']' << std::endl;
	cout << "Armor: [" << other.armor << '/'<< other.max_armor << "]\t\t\tPower: [" << other.power << '/'<< other.max_power << ']' << std::endl;
	return cout;
}



#endif