#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include <string>
#include <cmath>

#include "Surface.h"

/*DEFAULT VARIABLES*/
static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;
static const double DEFAULT_FPS_LIMIT = 30.0;
//static const int DEFAULT_NUM_OF_LAYERS = 1;//For Parallax if it is desired
const SDL_Color BLACK    = {  0,   0,   0, 0};
const SDL_Color RED      = {255,   0,   0, 0};
const SDL_Color WHITE    = {255, 255, 255, 0};
const SDL_Color GREEN    = {  0, 255,   0, 0};
const SDL_Color BLUE     = {  0,   0, 255, 0};
const SDL_Color GRAY     = {128, 128, 128, 0};
const SDL_Color DRAKGRAY = { 64,  64,  64, 0};
const SDL_Color YELLOW   = {255, 255,   0, 0};
const SDL_Color CYAN     = {  0, 255, 255, 0};
const SDL_Color ORANGE   = {255, 128,   0, 0};



/*System Variables*/
static SDL_Surface * WINDOW;
//static Surface LAYERS[DEFAULT_NUM_OF_LAYERS];//For Parallax if it is desired
static int WINDOW_WIDTH = DEFAULT_WINDOW_WIDTH;
static int WINDOW_HEIGHT = DEFAULT_WINDOW_HEIGHT;
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;
static const char * WINDOW_TITLE = "Game API Demo";
static const char * WINDOW_ICON_FILEPATH = "./Art/icon.bmp";
static const char * APPLICATION_VERSION = "0.0.0.0";
static bool IS_KEY_PRESSED[SDLK_LAST];
static double PI = 3.141592654;
static double TWOPI = 6.283185307;
static double TRIG_TABLE[72][2];

const int ROOM_WIDTH = 2000;
const int ROOM_HEIGHT = 2000;



/*System Functions*/
static void Reset_To_Default();
static void Build_Trig_Table();
static void Build_Key_Array();
static double GetTimePerFrame();
static SDL_Color Random_Color();



void Reset_To_Default() {
    WINDOW_WIDTH = DEFAULT_WINDOW_WIDTH;
    WINDOW_HEIGHT = DEFAULT_WINDOW_HEIGHT;
    WINDOW = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
}

void Build_Trig_Table() {
    for (int i = 0; i < 72; i++) {
        double temp = PI / 180 * (i * 5);
        TRIG_TABLE[i][0] = sin(temp);
        TRIG_TABLE[i][1] = cos(temp);
    }
}

void Build_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

void Clear_Window() {
    Surface::Fill(WINDOW, WINDOW_WIDTH, WINDOW_HEIGHT, BLACK);
}

double GetTimePerFrame() {
    return 1.0 / FPS_LIMIT;//return either FPS_LIMIT or FrameTime if FrameTime is greater (when program is running slower than it should)
}

SDL_Color Random_Color() {
  SDL_Color c = {rand() % 256, rand() % 256, rand() % 256, 0};
  return c;
}

#endif
