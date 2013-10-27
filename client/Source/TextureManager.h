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

class TextureManager {
public:
	static TextureManager * GetInstance();

	Texture * test;
	Texture * bullet;
	Texture * smoke;
	Texture * ship;
	Texture * background_main_menu;
	Texture * background_game;
	Texture * button;
	Texture * highlightedbutton;
	
	void Load();
	bool Resize(int, int);

	Fonts fonts;
	
private:
	TextureManager();
	~TextureManager();

	static TextureManager * instance;
};

extern TextureManager * surface_manager;

#endif
