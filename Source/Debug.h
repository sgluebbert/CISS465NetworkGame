#ifndef DEBUG_H
#define DEBUG_H



#include <iostream>

#include "Collidable.h"
#include "Drawable.h"
#include "Motor.h"
#include "RigidBody.h"
#include "Serializable.h"



template <class T>
std::ostream & operator<<(std::ostream & cout, const Circle<T> & other) {
	return cout;
}

template <class T>
std::ostream & operator<<(std::ostream & cout, const Rect<T> & other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Collidable * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Drawable * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Motor * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const RigidBody * other) {
	return cout;
}

std::ostream & operator<<(std::ostream & cout, const Serializable * other) {
	return cout;
}



#endif