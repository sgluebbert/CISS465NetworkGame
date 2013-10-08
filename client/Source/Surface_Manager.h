#ifndef SURFACE_MANAGER_H
#define SURFACE_MANAGER_H



#include "SDL_image.h"



class Surface_Manager {
public:
	static Surface_Manager * Get_Instance();
	static void Delete_Instance();

	SDL_Surface * Load_Image(const char*);

	SDL_Surface * test;
	SDL_Surface * bullet;
	SDL_Surface * smoke;
	SDL_Surface * ship;
	SDL_Surface * background_main_menu;
	SDL_Surface * background_game;
	SDL_Surface * button;
	SDL_Surface * highlightedbutton;
	
    static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
    static bool Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
		
	static Uint32 Map_RGB(SDL_Surface * Surf_Src, const SDL_Color & color);
		
    static bool DrawRect(SDL_Surface * Surf_Dest, int X, int Y, int W, int H, SDL_Color color);
    static bool Fill(SDL_Surface* Surf_Dest, int W, int H, SDL_Color color);

private:
	Surface_Manager();
	~Surface_Manager();

	static Surface_Manager * instance;
};



Surface_Manager * Surface_Manager::instance = NULL;



Surface_Manager::Surface_Manager() {
	test 					= Load_Image("./Art/Untitled.bmp");
	bullet 					= Load_Image("./Art/PPC_Shot.png");
	smoke 					= Load_Image("./Art/smoke.png");
	ship 					= Load_Image("./Art/ship01.png");
	background_main_menu 	= Load_Image("./Art/Main_Menu.bmp");
	background_game 		= Load_Image("./Art/Background.bmp");
	button 					= Load_Image("./Art/Button.bmp");
	highlightedbutton 		= Load_Image("./Art/HighlightedButton.bmp");
}

Surface_Manager::~Surface_Manager() {
	SDL_FreeSurface(test);
	SDL_FreeSurface(bullet);
	SDL_FreeSurface(smoke);
	SDL_FreeSurface(ship);
	SDL_FreeSurface(background_main_menu);
	SDL_FreeSurface(background_game);
	SDL_FreeSurface(button);
	SDL_FreeSurface(highlightedbutton);
}

Surface_Manager * Surface_Manager::Get_Instance() {
	if (instance == NULL)
		instance = new Surface_Manager();
	
	return instance;
}

void Surface_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}

SDL_Surface * Surface_Manager::Load_Image(const char *filename) {
	SDL_Surface *image = IMG_Load(filename);

	if (image == NULL)
		std::cerr << "Error loading: " << filename << '\n';

	return image;
}

bool Surface_Manager::Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
    if(Surf_Dest == NULL || Surf_Src == NULL) {
        return false;
    }
 
    SDL_Rect DestR;
 
    DestR.x = X;
    DestR.y = Y;
 
    SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
 
    return true;
}

bool Surface_Manager::Blit(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
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

Uint32 Surface_Manager::Map_RGB(SDL_Surface * Surf_Src, const SDL_Color & color) {
	return SDL_MapRGB(Surf_Src->format, color.r, color.g, color.b);
}

bool Surface_Manager::DrawRect(SDL_Surface * Surf_Dest, int X, int Y, int W, int H, SDL_Color color) {
    if(Surf_Dest == NULL)
        return false;
        
	SDL_Rect rect = {X,Y,W,H};
	Uint32 pixel = Map_RGB(Surf_Dest, color);
	SDL_FillRect(Surf_Dest, &rect, pixel);
 
    return true;
}

bool Surface_Manager::Fill(SDL_Surface * Surf_Dest, int W, int H, SDL_Color color) {
    if(Surf_Dest == NULL)
        return false;
        
	SDL_Rect rect = {0,0,W,H};
	Uint32 pixel = Map_RGB(Surf_Dest, color);
	SDL_FillRect(Surf_Dest, &rect, pixel);
 
    return true;
}


#endif
