/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef FONT_H
#define FONT_H

#include <string>

#include <SDL.h>
#include "SDL_ttf.h"

struct FontManager
{
	static FontManager* getInstance();
	SDL_Surface *Render(TTF_Font *f, std::string &s, const SDL_Color &c);
	SDL_Surface *Render(TTF_Font *f, const char *s, const SDL_Color &c);

	TTF_Font * title_font_12;
	TTF_Font * menu_font_24;
private:
	FontManager();
	~FontManager();

	static bool instanceFlag;
	static FontManager *single;

};

bool FontManager::instanceFlag = false;
FontManager* FontManager::single = NULL;

FontManager::FontManager() {
	if ( TTF_Init() != 0 ) {
		printf("Error in Font::Font: TTF_Init() != 0 with error %s\n", TTF_GetError());
		exit(1);
	}

	title_font_12 = TTF_OpenFont("./Font/microsbe.ttf", 12);
	if(!title_font_12) printf("TTF_OpenFont: %s\n", TTF_GetError());

	menu_font_24 = TTF_OpenFont("./Font/microsbe.ttf", 24);
	if(!menu_font_24) printf("TTF_OpenFont: %s\n", TTF_GetError());
}

FontManager::~FontManager() {

}

SDL_Surface *FontManager::Render(TTF_Font *f, const char *s, const SDL_Color &c) {
	// return TTF_RenderText_Solid(font, s, c);
	return TTF_RenderText_Blended(f, s, c);
}

SDL_Surface *FontManager::Render(TTF_Font *f, std::string &s, const SDL_Color &c) {
	return Render(f, s.c_str(), c);
}

FontManager* FontManager::getInstance() {
	if (!instanceFlag) {
		single = new FontManager();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}

// class Font {
// public:
// 	Font(const char fontfamily[] = "fonts/FreeSans.ttf", int size = 12) {
// 		atexit( TTF_Quit );
		
// 		if ( TTF_Init() != 0 ) {
// 			printf("Error in Font::Font: TTF_Init() != 0 with error %s\n", TTF_GetError());
// 			exit(1);
// 		}
		
// 		font = TTF_OpenFont(fontfamily, size);
// 	}

// 	~Font() {
// 		// TTF_CloseFont( font );
// 	}

// 	SDL_Surface * render(const char s[], const SDL_Color & c) {
// 		return TTF_RenderText_Solid(font, s, c);
// 	}

// 	SDL_Surface * render(const char s[], int r, int g, int b) {
// 	    SDL_Color c = {r, g, b, 0};
// 		return render(s, c);
// 	}

// 	/*SDL_Surface * render(const std::string s, const SDL_Color & c) {
//         const char * cs = s.c_str();
//         return TTF_RenderText_Solid(font, cs, c);
// 	}

// 	SDL_Surface * render(const std::string s, int r, int g, int b) {
//         const char * cs = s.c_str();
//         SDL_Color c = {r, g, b, 0};
//         return render(cs, c);
// 	}*/

// private:
// 	TTF_Font * font;
// };

#endif
