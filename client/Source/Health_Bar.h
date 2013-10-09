#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H



#include "Ship.h"



class Health_Bar {
public:
	Health_Bar();

	void Notify(Ship *);
	void Draw();

private:
	SDL_Surface * surface;
	SDL_Rect bounding_box;
	SDL_Rect progress_box;

	double MAX_WIDTH;
	double progress;
};



Health_Bar::Health_Bar() {
	bounding_box.x = 0;
	bounding_box.y = 0;
	bounding_box.w = 100;
	bounding_box.h = 25;
	progress_box.x = 2;
	progress_box.y = 2;
	progress_box.w = 96;
	progress_box.h = 21;

	MAX_WIDTH = 96;

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, bounding_box.w, bounding_box.h, 32, 0, 0, 0, 0);

	progress = 1.0;
}

void Health_Bar::Notify(Ship * player) {
	if (player == NULL)
		return;
	
	progress_box.w = MAX_WIDTH;
	progress = player->Get_Health();
	progress_box.w *= progress;

	SDL_FillRect(surface, &bounding_box, 0x808080);
	SDL_FillRect(surface, &progress_box, 0xFF0000);
}

void Health_Bar::Draw() {
	SDL_BlitSurface(surface, NULL, WINDOW, &bounding_box);
}



#endif