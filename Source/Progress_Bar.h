#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H



#include "Draw.h"
#include "Rect.h"



enum Health_Bar_State {	NORMAL_HEALTH_STATE, DAMAGE_HEALTH_STATE, REPAIR_HEALTH_STATE	};



class Progress_Bar {
public:
	Progress_Bar();

	Color back_color;
	Color border_color;
	Color normal_color;

	//Use integers since you can't have half a pixel, GUI only deals with pixels, not world units
	Rect<int> border_rect;
	Rect<int> bar_rect;

	double progress;

	void Set_Rect(int, int, int, int, int);

	virtual void Notify(double);
	virtual void Update(double);
	virtual void Draw();
};



class Health_Bar : public Progress_Bar {
public:
	Health_Bar();

	Color damage_color;
	Color repair_color;

	Health_Bar_State state;
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