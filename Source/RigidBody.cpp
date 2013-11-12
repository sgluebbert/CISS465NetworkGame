#include "RigidBody.h"



void Rigid_Body::Set_Inertia(float radius) {
	inertia = (2.0 * mass * radius * radius) / 3.0;
}

void Rigid_Body::Apply_Force(float new_force, float new_angle, float _x, float _y) {
	new_angle = new_angle * PI / 180;
    force += new_force;
    torque += ((_x * new_force * cos(new_angle)) + (_y * new_force * cos(new_angle)));
}

void Rigid_Body::Apply_Torque(float new_torque) {
    torque += new_torque;
}

void Rigid_Body::Calculate_Velocity(double dt) {
	velocity += (force / mass) * dt;
	force = 0.0;
}

void Rigid_Body::Calculate_Rotation(double dt) {
	rotation += (torque / inertia) * dt;
	torque = 0.0;
}

void Rigid_Body::Calculate_Vector(double dt) {
	angle += rotation * dt;

	dx = velocity * cos(angle * PI / 180) * dt;
	dy = velocity * sin(angle * PI / 180) * dt;
}

void Rigid_Body::Move(double dt) {
	x += dx;
	y -= dy;
}