#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include <SDL_net.h>
#include <string>
#include <cmath>

/*DEFAULT VARIABLES*/
static const SDL_Rect DEFAULT_WINDOW_BOUNDING_BOX = {0, 0, 800, 600};
static const double DEFAULT_FPS_LIMIT = 30.0;
//static const int DEFAULT_NUM_OF_LAYERS = 1;//For Parallax if it is desired

static const std::string DEFAULT_SERVER_IP = "192.168.0.102";
static const Uint16 DEFAULT_SERVER_PORT = 8080;


/*System Variables*/
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;

static std::string SERVER_IP = DEFAULT_SERVER_IP;
static Uint16 SERVER_PORT = DEFAULT_SERVER_PORT;

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


void Build_Trig_Table() {
    for (int i = 0; i < 72; i++) {
        double temp = 2 * PI * i / 72;
        TRIG_TABLE[i][0] = sin(temp);
        TRIG_TABLE[i][1] = cos(temp);
    }
}

void Build_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

double GetTimePerFrame() {
    return 1.0 / FPS_LIMIT;//return either FPS_LIMIT or FrameTime if FrameTime is greater (when program is running slower than it should)
}
#endif
