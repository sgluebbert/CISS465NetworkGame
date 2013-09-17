#ifndef APPSTATELOBBY_H
#define APPSTATELOBBY_H

#include "AppStateBase.h"
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include "Sound.h"
#include "SurfaceManager.h"
#include "System.h"
 
class AppStateLobby : public AppStateBase {
private:
        static AppStateBase * instance;
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
        
        static const char * BACKGROUND_FILENAME;
        static const char * MUSIC_FILENAME;

        UDPsocket sd;       /* Socket descriptor */
        UDPpacket *p;       /* Pointer to packet memory */
        AppStateLobby();
        ~AppStateLobby();
public:
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



AppStateBase * AppStateLobby::instance = NULL;

const char * AppStateLobby::BACKGROUND_FILENAME = "./Art/Background.bmp";
const char * AppStateLobby::MUSIC_FILENAME = "./Sound/Music/Battle.ogg";

AppStateLobby::AppStateLobby() {
}

AppStateLobby::~AppStateLobby() {
    /* Clean and exit */
    // SDLNet_FreePacket(p);
    // SDLNet_Quit();
}

void AppStateLobby::Initialize() { 
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    // /* Open a socket */
    // if (!(sd = SDLNet_UDP_Open(2000)))
    // {
    //     fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    //     exit(EXIT_FAILURE);
    // }

    //  Make space for the packet 
    // if (!(p = SDLNet_AllocPacket(512)))
    // {
    //     fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    //     exit(EXIT_FAILURE);
    // }

    SurfaceManager * surfaceManager = SurfaceManager::getInstance();
    background_surf = surfaceManager->background_main_menu01;
    //background_rect;
    
    MUSIC_STREAM.load(MUSIC_FILENAME);
    MUSIC_STREAM.play();
}

void AppStateLobby::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateLobby::Update() {
    
}

void AppStateLobby::Draw() {
    // Surface::Blit(WINDOW, background_surf, 0, 0);
    // SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, background_surf, &background_rect);
    
    // SDL_Rect rect = {200, 150, 400, 300};
    // Surface::DrawRect(WINDOW, rect, CYAN);
}

void AppStateLobby::Cleanup() {
    MUSIC_STREAM.stop();
}

AppStateBase * AppStateLobby::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateLobby();
    return instance;
}

void AppStateLobby::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

void AppStateLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

#endif
