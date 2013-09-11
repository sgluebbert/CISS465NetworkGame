/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef FONT_H
#define FONT_H

#include <string>

#include <SDL.h>
#include "SDL_ttf.h"

class Font {
public:
	Font(const char fontfamily[] = "fonts/FreeSans.ttf", int size = 12) {
		atexit( TTF_Quit );
		
		if ( TTF_Init() != 0 ) {
			printf("Error in Font::Font: TTF_Init() != 0 with error %s\n", TTF_GetError());
			exit(1);
		}
		
		font = TTF_OpenFont(fontfamily, size);
	}

	~Font() {
		TTF_CloseFont( font );
	}

	SDL_Surface * render(const char s[], const SDL_Color & c) {
		return TTF_RenderText_Solid(font, s, c);
	}

	SDL_Surface * render(const char s[], int r, int g, int b) {
	    SDL_Color c = {r, g, b, 0};
		return render(s, c);
	}

	/*SDL_Surface * render(const std::string s, const SDL_Color & c) {
        const char * cs = s.c_str();
        return TTF_RenderText_Solid(font, cs, c);
	}

	SDL_Surface * render(const std::string s, int r, int g, int b) {
        const char * cs = s.c_str();
        SDL_Color c = {r, g, b, 0};
        return render(cs, c);
	}*/

private:
	TTF_Font * font;
};

#endif
