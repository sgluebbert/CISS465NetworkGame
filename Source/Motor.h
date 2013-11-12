#ifndef MOTOR_H
#define MOTOR_H



class Motor {
public:
	virtual void Limit_Motor() = 0;

	float velocity_limit;
	float reverse_modifier;

	float rotation_limit;

	float force_motor;
	float torque_motor;
};



#endif