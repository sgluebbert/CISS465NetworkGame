#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
#include <SDL_image.h>

//
//Don't honestly know why this is a class rather than a collection of functions to mess with SDL_Surfaces
//

class Surface {
public:
        Surface();
		
        static SDL_Surface * OnLoad(const char *);
        
        static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface *, int, int);
		static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface *, int, int, int, int, int, int);
		
	    static Uint32 Map_RGB(SDL_Surface *, const SDL_Color &);
        static bool DrawRect(SDL_Surface *, SDL_Rect &, const SDL_Color &);
		static bool Fill(SDL_Surface *, SDL_Rect &, const SDL_Color &);
};
 
Surface::Surface() {
}
 
SDL_Surface * Surface::OnLoad(const char* filename) {
	SDL_Surface *image = IMG_Load(filename);
	if (image == NULL) {
		std::cerr << "Error loading: " << filename << '\n';
	}

	return image;
}
 
bool Surface::Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
    if(Surf_Dest == NULL || Surf_Src == NULL) {
        return false;
    }
 
    SDL_Rect DestR;
 
    DestR.x = X;
    DestR.y = Y;
 
    SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
 
    return true;
}

bool Surface::Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
    if(Surf_Dest == NULL || Surf_Src == NULL) {
        return false;
    }
 
    SDL_Rect DestR;
 
    DestR.x = X;
    DestR.y = Y;
 
    SDL_Rect SrcR;
 
    SrcR.x = X2;
    SrcR.y = Y2;
    SrcR.w = W;
    SrcR.h = H;
 
    SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);
 
    return true;
}

Uint32 Surface::Map_RGB(SDL_Surface * Surf_Src, const SDL_Color & color) {
	return SDL_MapRGB(Surf_Src->format, color.r, color.g, color.b);
}

bool Surface::DrawRect(SDL_Surface * surf_src, SDL_Rect & rect_to_draw, const SDL_Color & color) {
    if(surf_src == NULL)
        return false;
        
	Uint32 pixel = Map_RGB(surf_src, color);
	SDL_FillRect(surf_src, &rect_to_draw, pixel);
 
    return true;
}

bool Surface::Fill(SDL_Surface* surf_src, SDL_Rect & surf_rect, const SDL_Color & color) {
    if(surf_src == NULL)
        return false;
        
	Uint32 pixel = Map_RGB(surf_src, color);
	SDL_FillRect(surf_src, &surf_rect, pixel);
 
    return true;
}

#endif
