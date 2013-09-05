#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include <string>

#include "Surface.h"

/*DEFAULT VARIABLES*/
static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;
static const double DEFAULT_FPS_LIMIT = 30.0;
//static const int DEFAULT_NUM_OF_LAYERS = 1;
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
//static Surface LAYERS[DEFAULT_NUM_OF_LAYERS];
static int WINDOW_WIDTH = DEFAULT_WINDOW_WIDTH;
static int WINDOW_HEIGHT = DEFAULT_WINDOW_HEIGHT;
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;
static const char * WINDOW_TITLE = "Game API Demo";
static const char * WINDOW_ICON_FILEPATH = "./Art/icon.bmp";
static const char * APPLICATION_VERSION = "0.0.0.0";
static bool IS_KEY_PRESSED[2];


/*System Functions*/
static void resetToDefault();
static void buildTrigTable();
static void buildKeyArray();
static double GetTimePerFrame();

void resetToDefault() {
    WINDOW_WIDTH = DEFAULT_WINDOW_WIDTH;
    WINDOW_HEIGHT = DEFAULT_WINDOW_HEIGHT;
}

void buildTrigTable() {
}

void buildKeyArray() {
}

double GetTimePerFrame() {
    return FPS_LIMIT / 1000.0;
}

inline SDL_Color rand_color()
{
  SDL_Color c = {rand() % 256, rand() % 256, rand() % 256, 0};
  return c;
}

#endif
