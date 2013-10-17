#ifndef RADAR_H
#define RADAR_H


#include <vector>
#include <deque>

#include "Ship.h"

template <class T>
struct Vector
{
	Vector(T _x, T _y)
		: x(_x), y(_y)
	{}

	T x;
	T y;
};

class Radar {
public:
	Radar();

	void Notify(std::deque<Ship * > &);
	void Notify(Ship *);
	void Draw();

private:
	SDL_Surface * surface;
	SDL_Rect bounding_box;
	SDL_Rect radar_bounds;

	std::vector< Vector<float> > enemy_points;
	std::vector< Vector<float> > friendly_points;

	static const int X_OFFSET;
	static const int Y_OFFSET;
};



const int Radar::X_OFFSET = 350;
const int Radar::Y_OFFSET = 500;



Radar::Radar() {
	bounding_box.x = 0;
	bounding_box.y = 0;
	bounding_box.w = 100;
	bounding_box.h = 100;
	radar_bounds.x = bounding_box.x + 2;
	radar_bounds.y = bounding_box.y + 2;
	radar_bounds.w = bounding_box.w - 4;
	radar_bounds.h = bounding_box.h - 4;

	// surface = SDL_CreateRGBSurface(SDL_HWSURFACE, bounding_box.w, bounding_box.h, 32, 0, 0, 0, 0);
}

void Radar::Notify(std::deque<Ship * > & entities) {
	enemy_points.clear();

    for (int i = 0; i < entities.size(); i++) {
    	if (entities[i] == NULL)
    		continue;

    	Vector<float> v(entities[i]->x, entities[i]->y);
		v.x = X_OFFSET + (v.x / double(ROOM_WIDTH)) * radar_bounds.w;
		v.y = Y_OFFSET + (v.y / double(ROOM_HEIGHT)) * radar_bounds.h;
		enemy_points.push_back(v);
    }
}

void Radar::Notify(Ship * Ship) {
	if (Ship == NULL)
		return;

	friendly_points.clear();
	Vector<float> v(Ship->x, Ship->y);
	v.x = X_OFFSET + (v.x / double(ROOM_WIDTH)) * radar_bounds.w;
	v.y = Y_OFFSET + (v.y / double(ROOM_HEIGHT)) * radar_bounds.h;
	friendly_points.push_back(v);
}

void Radar::Draw() {
	bounding_box.x += X_OFFSET;
	bounding_box.y += Y_OFFSET;

	SurfaceManager::DrawRect(bounding_box.x, bounding_box.y, bounding_box.x + bounding_box.w, bounding_box.y + bounding_box.h, true, &BLACK);

	for (int i = 0; i < enemy_points.size(); i++)
	{
		SurfaceManager::DrawRect(enemy_points[i].x - 2, enemy_points[i].y - 2, enemy_points[i].x + 2, enemy_points[i].y + 2, true, &RED);
	}

	for (int i = 0; i < friendly_points.size(); i++)
	{
		SurfaceManager::DrawRect(friendly_points[i].x - 2, friendly_points[i].y - 2, friendly_points[i].x + 2, friendly_points[i].y + 2, true, &GREEN);
	}
	
	SurfaceManager::DrawRect(bounding_box.x, bounding_box.y, bounding_box.x + bounding_box.w, bounding_box.y + bounding_box.h, false, &WHITE);

	bounding_box.x -= X_OFFSET;
	bounding_box.y -= Y_OFFSET;
}



#endif