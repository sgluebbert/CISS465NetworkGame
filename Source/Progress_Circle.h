#ifndef PROGRESS_CIRCLE_H
#define PROGRESS_CIRCLE_H



#include "Circle.h"
#include "Progress_Bar.h"



class Progress_Circle : public Progress_Display {
public:
	Progress_Circle();
	
	Circle<double> border_circle;
	Circle<double> progress_circle;
	Circle<double> center_circle;

	double start_angle;

	virtual void Set_Circle(int, int, int, int, int, double);

	virtual void Notify(double);
	virtual void Update(double);
	virtual void Draw();
};



class Alignment_Circle : public Progress_Circle {
public:
	Alignment_Circle();

	Color lhs_color;
	Color rhs_color;

	double midangle;

	virtual void Set_Circle(int, int, int, int, int, double);

	virtual void Notify(double);
	virtual void Update(double);
	virtual void Draw();
};



class Health_Circle : public Progress_Circle {
public:
	Health_Circle();

	Color damage_color;
	Color repair_color;

	Health_Display_State state;
	double difference;
	double damage_equalization_rate;
	double repair_equalization_rate;

	//Use only if you want all bar colors to be the same hue
	void Set_Color(Color);

	virtual void Notify(double);
	virtual void Update(double);
	virtual void Draw();
};



#endif