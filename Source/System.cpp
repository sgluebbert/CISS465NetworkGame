#include "System.h"

Uint32 directedGameHost = 0;
Uint16 directedGamePort = 0;
std::string directedLobbyName("Undefined");
bool authenticated = false;
std::string username = "Guest";

Uint16 lobbySetPort = 0;
std::string lobbySetName = "General Lobby";
float lobbySetMapScale = 1.0;

char ConvertToChar(SDLKey sym, bool shiftOn)
{
    if (sym > SDLK_z || sym < SDLK_SPACE)
        return 0;

    if (sym >= SDLK_a && sym <= SDLK_z)
    {
        if (shiftOn)
            return 'A' + (sym - SDLK_a);
        else
            return 'a' + (sym - SDLK_a);
    }

    switch (sym)
    {
        case SDLK_SPACE:    return ' ';
        case SDLK_EXCLAIM:  return '!';
        case SDLK_QUOTEDBL: return '"';
        case SDLK_HASH:     return '#';
        case SDLK_DOLLAR:   return '$';
        case SDLK_AMPERSAND:return '&';
        case SDLK_QUOTE:    return (shiftOn ? '"' : '\'');
        case SDLK_LEFTPAREN:return '(';
        case SDLK_RIGHTPAREN:return ')';
        case SDLK_ASTERISK: return '*';
        case SDLK_PLUS:     return '+';
        case SDLK_COMMA:    return (shiftOn ? '<' : ',');
        case SDLK_MINUS:    return (shiftOn ? '_' : '-');
        case SDLK_PERIOD:   return (shiftOn ? '>' : '.');
        case SDLK_SLASH:    return (shiftOn ? '?' : '/');
        case SDLK_0:        return (shiftOn ? ')' : '0');
        case SDLK_1:        return (shiftOn ? '!' : '1');
        case SDLK_2:        return (shiftOn ? '@' : '2');
        case SDLK_3:        return (shiftOn ? '#' : '3');
        case SDLK_4:        return (shiftOn ? '$' : '4');
        case SDLK_5:        return (shiftOn ? '%' : '5');
        case SDLK_6:        return (shiftOn ? '^' : '6');
        case SDLK_7:        return (shiftOn ? '&' : '7');
        case SDLK_8:        return (shiftOn ? '*' : '8');
        case SDLK_9:        return (shiftOn ? '(' : '9');
        case SDLK_COLON:    return ':';
        case SDLK_SEMICOLON:return (shiftOn ? ':' : ';');
        case SDLK_LESS:     return '<';
        case SDLK_EQUALS:   return '=';
        case SDLK_GREATER:  return '>';
        case SDLK_QUESTION: return '?';
        case SDLK_LEFTBRACKET:return (shiftOn ? '{' : '[');
        case SDLK_BACKSLASH:return '\\';
        case SDLK_RIGHTBRACKET:return (shiftOn ? '}' : ']');
        case SDLK_CARET:    return '^';
        case SDLK_UNDERSCORE:return '_';
        case SDLK_BACKQUOTE:return (shiftOn ? '~' : '`');
    }

    return 0;
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
