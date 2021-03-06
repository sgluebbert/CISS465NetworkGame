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
	Texture * laser;
	Texture * missle;
	Texture * mine;
	Texture * smoke;
	Texture * shatter;
	Texture * ship_fighter;
	Texture * ship_fighter_c;
	Texture * ship_interceptor;
	Texture * ship_interceptor_c;
	Texture * ship_bomber;
	Texture * ship_bomber_c;
	Texture * ship_frigate;
	Texture * ship_frigate_c;
	Texture * spawn_point_icon;
	Texture * background_main_menu;
	Texture * background_game;
	Texture * background_game2;
	Texture * background_game3;
	Texture * button;
	Texture * highlightedbutton;
	Texture * planet_glow;
	Texture * red_planet;
	Texture * blue_planet;
	Texture * neutral_planet;
	Texture * field_glow_locked;
	Texture * field_glow_unlocked;
	Texture * moon;

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
