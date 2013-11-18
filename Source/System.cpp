#include "System.h"

std::string Get_Username() {
    static int i = -1;

    if (i == -1)
    {
        srand(time(NULL));
        i = rand() % 8;
    }

    switch (i)
    {
        case 0: return "Sam";
        case 1: return "Ujjwal";
        case 2: return "Mark";
        case 3: return "Trevin";
        case 4: return "Other1";
        case 5: return "Other2";
        case 6: return "Other3";
        case 7: return "Other4";
    }
}

/*
static void Reset_Window(SDL_Rect new_bounding_box = DEFAULT_WINDOW_BOUNDING_BOX) {
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

static void Initialize_Managers() {
    sound_manager = Sound_Manager::Get_Instance();
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

static void Initialize_Trig_Table() {
    for (int i = 0; i < 72; i++) {
        double temp = 2 * PI * i / 72;
        TRIG_TABLE[i][0] = sin(temp);
        TRIG_TABLE[i][1] = cos(temp);
    }
}

static void Initialize_Key_Array() {
    for (int i = SDLK_FIRST; i < SDLK_LAST; i++)
        IS_KEY_PRESSED[i] = false;
}

static bool Initialize_System() {
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

static void Cleanup_SDL() {
    Cleanup_SDL_Modules();

    // SDL_FreeSurface(WINDOW);
    // SDL_Quit();
}

static void Cleanup_SDL_Modules() {
    SDLNet_Quit();
}

static void Cleanup_Managers() {
    Sound_Manager::Delete_Instance();
}

static void Cleanup_UDP_Network() {
    //SDLNet_FreePacket(recieve);
    //SDLNet_FreePacket(send);
}

static void Cleanup_System() {
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
}*/
