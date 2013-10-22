#ifndef SURFACE_MANAGER_H
#define SURFACE_MANAGER_H

#include "Text.h"

struct Fonts
{
	Fonts();
	~Fonts();
	void init();

	TTF_Font *font_FreeMono_10;
	TTF_Font *font_FreeMono_12;
	TTF_Font *font_FreeMono_14;
	TTF_Font *font_FreeMono_16;
	TTF_Font *font_FreeMono_18;
	TTF_Font *font_FreeMono_20;

	TTF_Font *font_Impact_12;
	TTF_Font *font_Impact_14;
	TTF_Font *font_Impact_16;
	TTF_Font *font_Impact_18;
	TTF_Font *font_Impact_20;
};

class SurfaceManager {
public:
	static SurfaceManager * GetInstance();

	GLuint test;
	GLuint bullet;
	GLuint smoke;
	GLuint ship;
	GLuint background_main_menu;
	GLuint background_game;
	GLuint button;
	GLuint highlightedbutton;
	
	void Load();
	bool Resize(int, int);

	Fonts fonts;
	
private:
	SurfaceManager();
	~SurfaceManager();

	static SurfaceManager * instance;
};

extern SurfaceManager * surface_manager;

#endif
