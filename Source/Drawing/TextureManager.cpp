#include "TextureManager.h"

TextureManager * TextureManager::instance = NULL;
TextureManager * surface_manager = TextureManager::GetInstance();

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Load()
{
	fonts.init();

	bullet 					= new Texture("./Art/PPC_Shot.png");
	smoke 					= new Texture("./Art/smoke.png");
	shatter 				= new Texture("./Art/Shatter.png");
	ship 					= new Texture("./Art/ship.png");
	background_main_menu 	= new Texture("./Art/Main_Menu.png");
	background_game 		= new Texture("./Art/Background.png");
	background_game2 		= new Texture("./Art/Background2.png");
	button 					= new Texture("./Art/Button.png");
	highlightedbutton 		= new Texture("./Art/HighlightedButton.png");
	planet					= new Texture("./Art/Planet.png");
	red_planet				= new Texture("./Art/RedPlanet.png");
	blue_planet				= new Texture("./Art/BluePlanet.png");
	neutral_planet			= new Texture("./Art/NeutralPlanet.png");
	field_glow_locked 		= new Texture("./Art/PlanetGravityGlow.png");
	field_glow_unlocked     = new Texture("./Art/PlanetGravityGlow2.png");
	moon					= new Texture("./Art/moon.png");

}

bool TextureManager::Resize(int new_width, int new_height)
{
	// if (new_width < 1024)
	// 	new_width = 1024;

	// SDL_Surface *new_surface = SDL_SetVideoMode ( new_width, new_height, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE );

	// if (new_surface == NULL)
	// {
	// 	std::cerr << "An error occured while trying to resize window: " << SDL_GetError() << std::endl;
	// 	return false;
	// }

	// WINDOW_BOUNDING_BOX.w = new_width;
	// WINDOW_BOUNDING_BOX.h = new_height;
	// SDL_FreeSurface(surface);
	// surface = new_surface;
	// glViewport(0, 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h);
	return true;
}

TextureManager * TextureManager::GetInstance()
{
	if (instance == NULL)
		instance = new TextureManager();
	
	return instance;
}

Fonts::Fonts()
{
}

Fonts::~Fonts()
{
	// TTF_CloseFont(font_FreeMono_12);
	// std::cout << "killed fonts\n";
}

void Fonts::init()
{
	font_FreeMono_10 = TTF_OpenFont("Font/FreeMono.ttf", 10);
	if(!font_FreeMono_10) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_12 = TTF_OpenFont("Font/FreeMono.ttf", 12);
	if(!font_FreeMono_12) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_14 = TTF_OpenFont("Font/FreeMono.ttf", 14);
	if(!font_FreeMono_14) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_16 = TTF_OpenFont("Font/FreeMono.ttf", 16);
	if(!font_FreeMono_16) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_18 = TTF_OpenFont("Font/FreeMono.ttf", 18);
	if(!font_FreeMono_18) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_20 = TTF_OpenFont("Font/FreeMono.ttf", 20);
	if(!font_FreeMono_20) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_12 = TTF_OpenFont("Font/impact.ttf", 12);
	if(!font_Impact_12) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_14 = TTF_OpenFont("Font/impact.ttf", 14);
	if(!font_Impact_14) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_16 = TTF_OpenFont("Font/impact.ttf", 16);
	if(!font_Impact_16) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_18 = TTF_OpenFont("Font/impact.ttf", 18);
	if(!font_Impact_18) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_20 = TTF_OpenFont("Font/impact.ttf", 20);
	if(!font_Impact_20) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_80 = TTF_OpenFont("Font/impact.ttf", 80);
	if(!font_Impact_80) printf("TTF_OpenFont: %s\n", TTF_GetError());
}
