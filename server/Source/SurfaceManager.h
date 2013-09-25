#ifndef SURFACEMANGER_H
#define SURFACEMANGER_H

#include "SDL_image.h"

struct SurfaceManager {
	static SurfaceManager* getInstance();
	SDL_Surface *loadImage(const char*);

	SDL_Surface *ship01;
	SDL_Surface *bullet01;
	SDL_Surface *background_main_menu01, *background_game01;
	SDL_Surface *button01, *highlightedbutton01;

private:
	SurfaceManager();
	~SurfaceManager();

	static bool instanceFlag;
	static SurfaceManager *single;
};

bool SurfaceManager::instanceFlag = false;
SurfaceManager* SurfaceManager::single = NULL;



SurfaceManager::SurfaceManager() {
	bullet01 = loadImage("./Art/Untitled.bmp");
	ship01 = loadImage("./Art/ship01.png");
	background_main_menu01 = loadImage("./Art/Main_Menu.bmp");
	background_game01 = loadImage("./Art/Background.bmp");
	highlightedbutton01 = loadImage("./Art/HighlightedButton.bmp");
	button01 = loadImage("./Art/Button.bmp");
}

SurfaceManager::~SurfaceManager() {
	SDL_FreeSurface(ship01);
	SDL_FreeSurface(bullet01);
	SDL_FreeSurface(background_main_menu01);
	SDL_FreeSurface(background_game01);
	SDL_FreeSurface(button01);
	SDL_FreeSurface(highlightedbutton01);
}

SDL_Surface *SurfaceManager::loadImage(const char *filename) {
	SDL_Surface *image = IMG_Load(filename);
	if (image == NULL) {
		std::cerr << "Error loading: " << filename << '\n';
	}

	return image;
}

SurfaceManager* SurfaceManager::getInstance() {
	if (!instanceFlag) {
		single = new SurfaceManager();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}


#endif
