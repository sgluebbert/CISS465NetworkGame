#ifndef SYSTEM_H
#define SYSTEM_H



#include <string>
#include <cmath>

#include "SDL_net.h"
#include "Pallete.h"
#include "Font_Manager.h"
#include "Sound_Manager.h"
#include "Surface_Manager.h"



/*DEFAULT VARIABLES*/
static const SDL_Rect DEFAULT_WINDOW_BOUNDING_BOX = {0, 0, 800, 600};
static const double DEFAULT_FPS_LIMIT = 30.0;



/*System Variables*/
static SDL_Surface * WINDOW;
static SDL_Rect WINDOW_BOUNDING_BOX;
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;
static const char * WINDOW_TITLE = "CISS465 Project";
static const char * WINDOW_ICON_FILEPATH = "./Art/Icon.bmp";

static const char * APPLICATION_VERSION = "0.0.0.0";

static bool IS_KEY_PRESSED[SDLK_LAST];

static double PI = 3.141592654;
static double TWOPI = 6.283185307;
static double TRIG_TABLE[72][2];

static UDPsocket socket;
static UDPpacket * recieve;
static UDPpacket * send;

static IPaddress server_address;
static IPaddress client_address;

static Font_Manager * font_manager;
static Sound_Manager * sound_manager;
static Surface_Manager * surface_manager;

const int ROOM_WIDTH = 2000;
const int ROOM_HEIGHT = 2000;



/*System Functions*/
static void Reset_Window(SDL_Rect);

static bool Initialize_SDL();
static bool Initialize_SDL_Modules();
static void Initialize_Managers();
static bool Initialize_UDP_Network();

static void Initialize_Trig_Table();
static void Initialize_Key_Array();

static bool Initialize_System();

static void Cleanup_SDL();
static void Cleanup_SDL_Modules();
static void Cleanup_Managers();
static void Cleanup_UDP_Network();

static void Cleanup_System();

static void Clear_Window();
static SDL_Color Random_Color();



void Reset_Window(SDL_Rect new_bounding_box = DEFAULT_WINDOW_BOUNDING_BOX) {
    WINDOW_BOUNDING_BOX = new_bounding_box;
    WINDOW = SDL_SetVideoMode(WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF);
}



bool Initialize_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;

    if (!Initialize_SDL_Modules())
        return false;

    return true;
}

bool Initialize_SDL_Modules() {
    if (TTF_Init() < 0 )
        return false;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    if (SDLNet_Init() < 0)
        return false;

    return true;
}

void Initialize_Managers() {
    font_manager = Font_Manager::Get_Instance();
    sound_manager = Sound_Manager::Get_Instance();
    surface_manager = Surface_Manager::Get_Instance();
}

bool Initialize_UDP_Network() {
    //client_address.address = 082304324;
    client_address.port = 0;

    if (!(socket = SDLNet_UDP_Open(client_address.port)))
        return false;
 
    /* Make space for the packet */
    if (!(recieve = SDLNet_AllocPacket(512)))
        return false;
 
    /* Make space for the packet */
    if (!(send = SDLNet_AllocPacket(512)))
        return false;

    return true;
}

void Initialize_Trig_Table() {
    for (int i = 0; i < 72; i++) {
        double temp = 2 * PI * i / 72;
        TRIG_TABLE[i][0] = sin(temp);
        TRIG_TABLE[i][1] = cos(temp);
    }
}

void Initialize_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

bool Initialize_System() {
    if (!Initialize_SDL())
        return false;

    if (!Initialize_UDP_Network())
        return false;

    Initialize_Managers();

    SDL_WM_SetCaption(WINDOW_TITLE, NULL);
    SDL_WM_SetIcon(SDL_LoadBMP(WINDOW_ICON_FILEPATH), NULL);

    if((WINDOW = SDL_SetVideoMode(WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
        return false;
    
    Initialize_Trig_Table();
    Initialize_Key_Array();

    return true;
}

void Cleanup_SDL() {
    Cleanup_SDL_Modules();

    SDL_FreeSurface(WINDOW);
    SDL_Quit();
}

void Cleanup_SDL_Modules() {
    SDLNet_Quit();
}

void Cleanup_Managers() {
    Font_Manager::Delete_Instance();
    Sound_Manager::Delete_Instance();
    Surface_Manager::Delete_Instance();
}

void Cleanup_UDP_Network() {
    SDLNet_FreePacket(recieve);
    //SDLNet_FreePacket(send);
}

void Cleanup_System() {
    Cleanup_Managers();
    Cleanup_UDP_Network();
    Cleanup_SDL();
}



void Clear_Window() {
    SDL_FillRect(WINDOW, &WINDOW_BOUNDING_BOX, 0x000000);
}

SDL_Color Random_Color() {
  SDL_Color c = {rand() % 256, rand() % 256, rand() % 256, 0};
  return c;
}



#endif
