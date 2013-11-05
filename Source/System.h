#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <cmath>

#include "SDL_net.h"
#include "Pallete.h"
// #include "SoundManager.h"
#include "TextureManager.h"
#include "Networking/Network.h"

/*DEFAULT VARIABLES*/
static const double DEFAULT_FPS_LIMIT = 30.0;



/*System Variables*/
static SDL_Surface * WINDOW;
static double FPS_LIMIT = DEFAULT_FPS_LIMIT;
static const char * WINDOW_TITLE = "CISS465 Project";
static const char * WINDOW_ICON_FILEPATH = "./Art/Icon.bmp";

static const char * APPLICATION_VERSION = "0.0.0.0";

static bool IS_KEY_PRESSED[SDLK_LAST];

static double PI = 3.141592654;
static double TWOPI = 6.283185307;
static double TRIG_TABLE[72][2];

// static UDPsocket socket;
// static UDPpacket * recieve;
// static UDPpacket * send;

// static IPaddress server_address;
// static IPaddress client_address;

// Network* NetworkFactory::instance = NULL;
// NetworkType NetworkFactory::networkType = UNDEFINED;
// Network *network = NetworkFactory::getInstance();

// static Sound_Manager * sound_manager;

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

//const char *server_ipaddress = NULL;

void Reset_Window(SDL_Rect new_bounding_box = DEFAULT_WINDOW_BOUNDING_BOX) {
    WINDOW_BOUNDING_BOX = new_bounding_box;
    WINDOW = SDL_SetVideoMode(WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF);
}



// bool Initialize_SDL() {
//     if (SDL_Init(SDL_INIT_VIDEO) < 0)
//         return false;

//     if (!Initialize_SDL_Modules())
//         return false;

//     return true;
// }

// bool Initialize_SDL_Modules() {
//     if (TTF_Init() < 0 )
//         return false;

//     if (SDL_Init(SDL_INIT_AUDIO) < 0)
//         return false;

//     if (SDLNet_Init() < 0)
//         return false;

//     return true;
// }

void Initialize_Managers() {
    // sound_manager = Sound_Manager::Get_Instance();
}

// bool Initialize_UDP_Network() {

//     client_address.port = 0;

//     if (!(socket = SDLNet_UDP_Open(client_address.port)))
//         return false;
 
//     if (!(recieve = SDLNet_AllocPacket(512)))
//         return false;
 
//     if (!(send = SDLNet_AllocPacket(512)))
//         return false;

//     return true;
// }

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
    // if (!Initialize_SDL())
    //     return false;

    // if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    // {
    //     printf("Unable to initialize SDL: %s\n", SDL_GetError());
    //     exit(1);
    // }

    // if (TTF_Init() != 0 )
    // {
    //     printf("Unable to initialize TTF: %s\n", TTF_GetError());
    //     exit(1);
    // }

    // WINDOW_BOUNDING_BOX = DEFAULT_WINDOW_BOUNDING_BOX;

    // // SDL_ShowCursor(false);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // SDL_WM_SetCaption("Not Named", NULL);
    // surface_manager->surface = SDL_SetVideoMode ( WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE );// | SDL_FULLSCREEN);

    // if ( surface_manager->surface == NULL ) 
    // {
    //     std::cerr << "Unable to set " << WINDOW_BOUNDING_BOX.w << "x" << WINDOW_BOUNDING_BOX.h << " video: " << SDL_GetError() << std::endl;
    //     exit(1);
    // }

    // atexit(TTF_Quit);
    // atexit(SDL_Quit);

    // glClearColor(0.f, 0.f, 0.f, 0.f);
    // glViewport(0, 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_POINT_SMOOTH);

    // // Antialiasing on and amount
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 3);

    // //Check for error
    // GLenum error = glGetError();
    // if( error != GL_NO_ERROR )
    // {
    //     std::cerr << "Error initializing OpenGL: " << gluErrorString(error) << std::endl;
    //     exit(1);
    // }
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

    // SDL_FreeSurface(WINDOW);
    // SDL_Quit();
}

void Cleanup_SDL_Modules() {
    SDLNet_Quit();
}

void Cleanup_Managers() {
    // Sound_Manager::Delete_Instance();
}

void Cleanup_UDP_Network() {
    //SDLNet_FreePacket(recieve);
    //SDLNet_FreePacket(send);
}

void Cleanup_System() {
    Cleanup_Managers();
    //Cleanup_UDP_Network();
    Cleanup_SDL();
}

inline float read_float(unsigned char *buffer)
{
    float temp;
    unsigned int temp2 = SDLNet_Read32(buffer);
    memcpy(&temp, &temp2, sizeof(float));
    return temp;
}

bool point_in_rect(float x, float y, float x1, float y1, float x2, float y2) {
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

// void Clear_Window() {
//     SDL_FillRect(WINDOW, &WINDOW_BOUNDING_BOX, 0x000000);
// }

SDL_Color Random_Color() {
  SDL_Color c = {rand() % 256, rand() % 256, rand() % 256, 0};
  return c;
}



#endif
