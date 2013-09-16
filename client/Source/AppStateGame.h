#ifndef APPSTATEGAME_H
#define APPSTATEGAME_H

#include "AppStateBase.h"
#include "Sound.h"
#include "SurfaceManager.h"
#include "System.h"
 
class AppStateGame : public AppStateBase {
private:
        static AppStateBase * instance;
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
        
        static const char * BACKGROUND_FILENAME;
        static const char * MUSIC_FILENAME;
        
public:
        AppStateGame();
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



AppStateBase * AppStateGame::instance = NULL;

const char * AppStateGame::BACKGROUND_FILENAME = "./Art/Background.bmp";
const char * AppStateGame::MUSIC_FILENAME = "./Sound/Music/Battle.ogg";

AppStateGame::AppStateGame() {
}

void AppStateGame::Initialize() {
    background_surf = NULL;//LoadImage(BACKGROUND_FILENAME);
    //background_rect;
    
    MUSIC_STREAM.load(MUSIC_FILENAME);
    MUSIC_STREAM.play();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
}

void AppStateGame::Draw() {
    SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, background_surf, &background_rect);
    
    SDL_Rect rect = {200, 150, 400, 300};
    Surface::DrawRect(WINDOW, rect, CYAN);
}

void AppStateGame::Cleanup() {
    MUSIC_STREAM.stop();
}

AppStateBase * AppStateGame::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGame();
    return instance;
}

void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_TAB:  AppStateEvent::New_Event(APPSTATE_NONE);    break;
    default:        AppStateEvent::New_Event(APPSTATE_MENU);    break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

#endif
