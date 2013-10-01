#ifndef RADAR_H
#define RADAR_H



#include <deque>

#include "Entity.h"



class Radar {
public:
	Radar();

	void Notify(std::deque<Entity * > &);
	void Notify(Entity *);
	void Draw();

private:
	SDL_Surface * surface;
	SDL_Rect bounding_box;
	SDL_Rect radar_bounds;

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

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, bounding_box.w, bounding_box.h, 32, 0, 0, 0, 0);
}

void Radar::Notify(std::deque<Entity * > & entities) {
	SDL_FillRect(surface, &bounding_box, 0x808080);
	SDL_FillRect(surface, &radar_bounds, 0x000000);

	SDL_Rect temp;

    for (int i = 0; i < entities.size(); i++) {
    	if (entities[i] == NULL)
    		continue;

		temp = entities[i]->Get_Bounding_Box();
		temp.x = (temp.x / double(ROOM_WIDTH)) * radar_bounds.w + 2;
		temp.y = (temp.y / double(ROOM_HEIGHT)) * radar_bounds.h + 2;
		temp.w = 4;
		temp.h = 4;
		SDL_FillRect(surface, &temp, 0xFF0000);
    }
}

void Radar::Notify(Entity * entity) {
	if (entity == NULL)
		return;

	SDL_Rect temp = entity->Get_Bounding_Box();
	temp.x = (temp.x / double(ROOM_WIDTH)) * radar_bounds.w + 2;
	temp.y = (temp.y / double(ROOM_HEIGHT)) * radar_bounds.h + 2;
	temp.w = 4;
	temp.h = 4;
	SDL_FillRect(surface, &temp, 0x00FF00);
}

void Radar::Draw() {
	bounding_box.x += X_OFFSET;
	bounding_box.y += Y_OFFSET;

	SDL_BlitSurface(surface, NULL, WINDOW, &bounding_box);

	bounding_box.x -= X_OFFSET;
	bounding_box.y -= Y_OFFSET;
}



#endif