#ifndef SYSTEM_H
#define SYSTEM_H



#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "Math.h"
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

static const int ROOM_WIDTH = 2000;
static const int ROOM_HEIGHT = 2000;

extern Uint32 directedGameHost;
extern Uint16 directedGamePort;
extern std::string directedLobbyName;
extern bool authenticated;
extern std::string username;

extern Uint16 lobbySetPort;
extern std::string lobbySetName;
extern float lobbySetMapScale;


/*System Functions*/
static void Initialize_Key_Array();

static bool Initialize_System();

static void Clear_Window();
static SDL_Color Random_Color();

static int Parse_Line(char *);
static int Get_Memory_Usage();



void Initialize_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

bool Initialize_System() {
    InitWindow();

    surface_manager->Load();

    SDL_WM_SetCaption(WINDOW_TITLE, NULL);
    SDL_WM_SetIcon(SDL_LoadBMP(WINDOW_ICON_FILEPATH), NULL);
    
    Initialize_Trig_Table();
    Initialize_Key_Array();

    return true;
}
    

int Parse_Line(char * line) {
    int i = strlen(line);

    while (*line < '0' || *line > '9')
        line++;

    line[i-3] = '\0';
    i = atoi(line);

    return i;
}


int Get_Memory_Usage() { //Note: this value is in KB!
    FILE * file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];


    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = Parse_Line(line);
            break;
        }
    }

    fclose(file);
    return result;
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

char ConvertToChar(SDLKey sym, bool shiftOn = false);

#endif
