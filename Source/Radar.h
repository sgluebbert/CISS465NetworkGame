#ifndef RADAR_H
#define RADAR_H



#include <deque>

#include "Rect.h"
#include "Ship.h"
#include "System.h"



class Radar {
public:
	Radar();

	void Notify(const std::deque<Ship * > &);
	void Notify(Ship *);
	void Draw();

private:
	Rect<int> bounding_box;
	Rect<int> radar_bounds;

	Ship * player_ship;
	std::deque<Ship *> ships;

	static const int X_OFFSET;
	static const int Y_OFFSET;
	static const int BORDER_WIDTH;
};



#endif