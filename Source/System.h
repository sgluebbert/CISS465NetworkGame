#ifndef SYSTEM_H
#define SYSTEM_H



#include <string>
#include <cmath>

#include "SDL_net.h"
#include "Math.h"
#include "SoundManager.h"
#include "Drawing/TextureManager.h"



/*DEFAULT VARIABLES*/
static const double DEFAULT_FPS_LIMIT = 30.0;



/*System Variables*/
static SDL_Surface * WINDOW;
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;
static const char * WINDOW_TITLE = "CISS465 Project";
static const char * WINDOW_ICON_FILEPATH = "./Art/Icon.bmp";

static const char * APPLICATION_VERSION = "0.0.0.0";

static bool IS_KEY_PRESSED[SDLK_LAST];

// static UDPsocket socket;
// static UDPpacket * recieve;
// static UDPpacket * send;

// static IPaddress server_address;
// static IPaddress client_address;

// Network* NetworkFactory::instance = NULL;
// NetworkType NetworkFactory::networkType = UNDEFINED;
// Network *network = NetworkFactory::getInstance();

static Sound_Manager * sound_manager;

static const int ROOM_WIDTH = 2000;
static const int ROOM_HEIGHT = 2000;



/*System Functions*/
static void Initialize_Managers();
static void Initialize_Key_Array();

static bool Initialize_System();

static void Cleanup_SDL();
static void Cleanup_SDL_Modules();
static void Cleanup_Managers();

static void Cleanup_System();

static void Clear_Window();
static SDL_Color Random_Color();



void Initialize_Managers() {
    sound_manager = Sound_Manager::Get_Instance();
}

void Initialize_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

bool Initialize_System() {
    InitWindow();

    // std::cout << "Ready\n";
    surface_manager->Load();

    // if (!Initialize_UDP_Network())
    //     return false;

    Initialize_Managers();

    SDL_WM_SetCaption(WINDOW_TITLE, NULL);
    SDL_WM_SetIcon(SDL_LoadBMP(WINDOW_ICON_FILEPATH), NULL);

    // if((WINDOW = SDL_SetVideoMode(WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    //     return false;
    
    Initialize_Trig_Table();
    Initialize_Key_Array();

    return true;
}

void Cleanup_SDL() {
    Cleanup_SDL_Modules();
}

void Cleanup_SDL_Modules() {
    SDLNet_Quit();
}

void Cleanup_Managers() {
    Sound_Manager::Delete_Instance();
}

void Cleanup_System() {
    Cleanup_Managers();
    Cleanup_SDL();
}

inline float read_float(unsigned char *buffer)
{
    float temp;
    unsigned int temp2 = SDLNet_Read32(buffer);
    memcpy(&temp, &temp2, sizeof(float));
    return temp;
}

static bool point_in_rect(float x, float y, float x1, float y1, float x2, float y2) {
    if(y < y1 || y > y2 || x < x1 || x > x2)
        return false;

    return true;
}

inline unsigned int min(unsigned int x, unsigned int y)
{
    return (x < y ? x : y);
}

inline float point_distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}



#endif
