#ifndef RIGIDBODY_H
#define RIGIDBODY_H



#include <deque>

#include "Math.h"



static const float FRICTION_COEFFICIENT = 0.96;



class Rigid_Body {
public:
	void Set_Inertia(float);
	void Apply_Force(float, float, float, float);
	void Apply_Torque(float);
	void Calculate_Velocity(double);
	void Calculate_Rotation(double);
	void Calculate_Vector(double);
	void Move(double);
	virtual void Update(double) = 0;

	float x, y;
    float angle;

    float mass;
    float force;

    float velocity;

    float inertia;
    float torque;

    float rotation;

    float dx, dy;

    static std::deque<Rigid_Body *> objects;

private:
};



#endif