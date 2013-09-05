#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
//#include <SDL_image.h>

//
//Don't honestly know why this is a class rather than a collection of functions to mess with SDL_Surfaces
//

class Surface {
public:
        Surface();
		
        static SDL_Surface * OnLoad(char* File);
        static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
		static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
		static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);
		static bool DrawRect(SDL_Surface* Surf_Dest, int X, int Y, int W, int H);
		static bool Fill(SDL_Surface* Surf_Dest, int X, int Y, int W, int H, int R, int G, int B, int A);
};
 
Surface::Surface() {
}
 
SDL_Surface * Surface::OnLoad(char* File) {
    SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Return = NULL; 
    
    if((Surf_Temp = SDL_LoadBMP(File)) == NULL)
        return NULL;
 
    Surf_Return = SDL_DisplayFormat(Surf_Temp);
    SDL_FreeSurface(Surf_Temp);
 
    return Surf_Return;
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

bool Surface::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B) {
    if(Surf_Dest == NULL) {
        return false;
    }

    SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format, R, G, B));

    return true;
}

bool Surface::DrawRect(SDL_Surface * Surf_Dest, int X, int Y, int W, int H) {
    if(Surf_Dest == NULL)
        return false;
        
	SDL_Rect rect = {X,Y,W,H};
	//Uint32 pixel = map_rgb(color);
	//SDL_FillRect(Surf_Dest, &rect, pixel);
 
    return true;
}

bool Surface::Fill(SDL_Surface* Surf_Dest, int X, int Y, int W, int H, int R, int G, int B, int A) {
    if(Surf_Dest == NULL)
        return false;
        
	SDL_Rect rect = {X,Y,W,H};
	//Uint32 pixel = map_rgb(color);
	//SDL_FillRect(Surf_Dest, &rect, pixel);
 
    return true;
}

#endif
