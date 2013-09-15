#ifndef SURFACEMANGER_H
#define SURFACEMANGER_H

#include "SDL_image.h"

struct SurfaceManager {
	static SurfaceManager* getInstance();
	SDL_Surface *loadImage(const char*);

	SDL_Surface *pawnSurface1;

private:
	SurfaceManager();
	~SurfaceManager();

	static bool instanceFlag;
	static SurfaceManager *single;
};

bool SurfaceManager::instanceFlag = false;
SurfaceManager* SurfaceManager::single = NULL;



SurfaceManager::SurfaceManager() {
	pawnSurface1 = loadImage("./Art/ship01.png");
}

SurfaceManager::~SurfaceManager() {
	SDL_FreeSurface(pawnSurface1);
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