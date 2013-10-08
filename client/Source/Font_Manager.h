#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H



#include <string>

#include "SDL.h"
#include "SDL_ttf.h"



class Font_Manager {
public:
	static Font_Manager * Get_Instance();
	static void Delete_Instance();
	
	static SDL_Surface * Render(TTF_Font *f, std::string &s, const SDL_Color &c);
	static SDL_Surface * Render(TTF_Font *f, const char *s, const SDL_Color &c);

	TTF_Font * chat_window_font;
	TTF_Font * menu_font;
	TTF_Font * bumper_font;
	
private:
	Font_Manager();
	~Font_Manager();

	static Font_Manager * instance;
};



Font_Manager * Font_Manager::instance = NULL;



Font_Manager::Font_Manager() {
	chat_window_font = TTF_OpenFont("./Font/microsbe.ttf", 8);
	if(!chat_window_font)
	    printf("TTF_OpenFont: %s\n", TTF_GetError());

	menu_font = TTF_OpenFont("./Font/microsbe.ttf", 12);
	if(!menu_font)
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
	    
	bumper_font = TTF_OpenFont("./Font/microsbe.ttf", 48);
	if(!bumper_font)
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
}

Font_Manager::~Font_Manager() {
    TTF_CloseFont(chat_window_font);
    TTF_CloseFont(menu_font);
    TTF_CloseFont(bumper_font);
}

Font_Manager * Font_Manager::Get_Instance() {
	if (instance == NULL)
		instance = new Font_Manager();
		
	return instance;
}

void Font_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}

SDL_Surface * Font_Manager::Render(TTF_Font * f, const char * s, const SDL_Color & c) {
	//return TTF_RenderText_Solid(font, s, c);
	return TTF_RenderText_Blended(f, s, c);
}

SDL_Surface * Font_Manager::Render(TTF_Font * f, std::string & s, const SDL_Color & c) {
	//return TTF_RenderText_Solid(font, s, c);
	return TTF_RenderText_Blended(f, s.c_str(), c);
}



#endif
