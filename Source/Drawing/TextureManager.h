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
	TTF_Font *font_Impact_80;
};

class TextureManager {
public:
	static TextureManager * GetInstance();

	Texture * bullet;
	Texture * smoke;
	Texture * shatter;
	Texture * ship;
	Texture * background_main_menu;
	Texture * background_game;
	Texture * background_game2;
	Texture * button;
	Texture * highlightedbutton;
	Texture * planet;
	Texture * red_planet;
	Texture * blue_planet;
	Texture * neutral_planet;

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
