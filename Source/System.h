#ifndef SYSTEM_H
#define SYSTEM_H



#include <string>
#include <cmath>

#include "SDL_net.h"
#include "Rect.h"



/*Default Variables*/
static const Rect<double> DEFAULT_WINDOW_BOUNDING_BOX;
static const double DEFAULT_FPS_LIMIT = 30.0;

/*Neutral Colors*/
static const Uint32 BLACK_PIXEL             = 0x000000;
static const Uint32 DARKGRAY_PIXEL          = 0x404040;
static const Uint32 GRAY_PIXEL              = 0x808080;
static const Uint32 LIGHTGRAY_PIXEL         = 0xC0C0C0;
static const Uint32 WHITE_PIXEL             = 0xFFFFFF;

/*Shades of Red Colors*/
static const Uint32 TRUE_RED_PIXEL          = 0xFF0000;
static const Uint32 BRIGHT_RED_PIXEL        = 0xC00000;
static const Uint32 RED_PIXEL               = 0x800000;
static const Uint32 DARK_RED_PIXEL          = 0x400000;

/*Shades of Yellow Colors*/
static const Uint32 TRUE_YELLOW_PIXEL       = 0xFFFF00;
static const Uint32 BRIGHT_YELLOW_PIXEL     = 0xC0C000;
static const Uint32 YELLOW_PIXEL            = 0x808000;
static const Uint32 DARK_YELLOW_PIXEL       = 0x404000;

/*Shades of Green Colors*/
static const Uint32 TRUE_GREEN_PIXEL        = 0x00FF00;
static const Uint32 BRIGHT_GREEN_PIXEL      = 0x00C000;
static const Uint32 GREEN_PIXEL             = 0x008000;
static const Uint32 DARK_GREEN_PIXEL        = 0x004000;

/*Shades of Teal Colors*/
static const Uint32 CYAN_PIXEL              = 0x00FFFF;
static const Uint32 BRIGHT_TEAL_PIXEL       = 0x00C0C0;
static const Uint32 TEAL_PIXEL              = 0x008080;
static const Uint32 DARK_TEAL_PIXEL         = 0x004040;

/*Shades of Blue Colors*/
static const Uint32 TRUE_BLUE_PIXEL         = 0x0000FF;
static const Uint32 BRIGHT_BLUE_PIXEL       = 0x0000C0;
static const Uint32 BLUE_PIXEL              = 0x000080;
static const Uint32 DARK_BLUE_PIXEL         = 0x000040;

/*Shades of Purple Colors*/
static const Uint32 TRUE_PURPLE_PIXEL       = 0xFF00FF;
static const Uint32 BRIGHT_PURPLE_PIXEL     = 0xC000C0;
static const Uint32 PURPLE_PIXEL            = 0x800080;
static const Uint32 DARK_PURPLE_PIXEL       = 0x400040;

/*Transparent Colors*/
static const SDL_Color CLEAR_BLACK      = {  0,   0,   0, 255};
static const SDL_Color CLEAR_WHITE      = {255, 255, 255, 255};

/*Neutral Colors*/
static const SDL_Color BLACK            = {  0,   0,   0,   0};
static const SDL_Color DARK_GRAY        = { 64,  64,  64,   0};
static const SDL_Color GRAY             = {128, 128, 128,   0};
static const SDL_Color LIGHT_GRAY       = {192, 192, 192,   0};
static const SDL_Color WHITE            = {255, 255, 255,   0};

/*Shades of Red Colors*/
static const SDL_Color TRUE_RED         = {255,   0,   0,   0};
static const SDL_Color BRIGHT_RED       = {192,   0,   0,   0};
static const SDL_Color RED              = {128,   0,   0,   0};
static const SDL_Color DARK_RED         = { 64,   0,   0,   0};

/*Shades of Yellow Colors*/
static const SDL_Color TRUE_YELLOW      = {255, 255,   0,   0};
static const SDL_Color BRIGHT_YELLOW    = {192, 192,   0,   0};
static const SDL_Color YELLOW           = {128, 128,   0,   0};
static const SDL_Color DARK_YELLOW      = { 64,  64,   0,   0};

/*Shades of Green Colors*/
static const SDL_Color TRUE_GREEN       = {  0, 255,   0,   0};
static const SDL_Color BRIGHT_GREEN     = {  0, 192,   0,   0};
static const SDL_Color GREEN            = {  0, 128,   0,   0};
static const SDL_Color DARK_GREEN       = {  0,  64,   0,   0};

/*Shades of Teal Colors*/
static const SDL_Color CYAN             = {  0, 255, 255,   0};
static const SDL_Color BRIGHT_TEAL      = {  0, 192, 192,   0};
static const SDL_Color TEAL             = {  0, 128, 128,   0};
static const SDL_Color DARK_TEAL        = {  0,  64,  64,   0};

/*Shades of Blue Colors*/
static const SDL_Color TRUE_BLUE        = {  0,   0, 255,   0};
static const SDL_Color BRIGHT_BLUE      = {  0,   0, 192,   0};
static const SDL_Color BLUE             = {  0,   0, 128,   0};
static const SDL_Color DARK_BLUE        = {  0,   0,  64,   0};

/*Shades of Purple Colors*/
static const SDL_Color TRUE_PURPLE      = {255,   0, 255,   0};
static const SDL_Color BRIGHT_PURPLE    = {192,   0, 192,   0};
static const SDL_Color PURPLE           = {128,   0, 128,   0};
static const SDL_Color DARK_PURPLE      = { 64,   0,  64,   0};



/*System Variables*/
static Rect<double> WINDOW_BOUNDING_BOX;
static SDL_Surface * WINDOW;
static const char * WINDOW_TITLE = "Game API Demo";
static const char * WINDOW_ICON_FILEPATH = "./Art/Icon.bmp";

static double FPS_LIMIT = DEFAULT_FPS_LIMIT;

static const char * APPLICATION_VERSION = "0.0.0.0";

static UDPsocket socket;
static UDPpacket * recieve;
static UDPpacket * send;

static IPaddress client_address;
static IPaddress server_address;

static bool IS_KEY_PRESSED[SDLK_LAST];

static double PI = 3.141592654;
static double TWOPI = 6.283185307;
static double TRIG_TABLE[72][2];



/*System Functions Prototypes*/
static void Reset_Window(Rect<double>);

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



/*System Function Bodies*/
void Reset_Window(Rect<double> new_bounding_box) {
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
    /*if (TTF_Init() < 0 )
        return false;*/

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    if (SDLNet_Init() < 0)
        return false;

    return true;
}

bool Initialize_UDP_Network() {
    //client_address.address = 082304324;
    client_address.port = 8080;

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

void Cleanup_UDP_Network() {
    SDLNet_FreePacket(recieve);
    SDLNet_FreePacket(send);
}

void Cleanup_System() {
    Cleanup_UDP_Network();
    Cleanup_SDL();
}



/*Helper Functions*/
void Clear_Window() {
}

SDL_Color Random_Color() {
  SDL_Color c = {rand() % 256, rand() % 256, rand() % 256, 0};
  return c;
}



#endif
