#ifndef APPSTATEMENU_H
#define APPSTATEMENU_H

#include "AppStateBase.h"
#include "Menu.h"
#include "Sound.h"
#include "SurfaceManager.h"
#include "System.h"
 
class AppStateMenu : public AppStateBase {
private:
        static AppStateBase * instance;
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
        
        static const char * BACKGROUND_FILENAME;
        static const char * MUSIC_FILENAME;
        
public:
        AppStateMenu();
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



AppStateBase * AppStateMenu::instance = NULL;

const char * AppStateMenu::BACKGROUND_FILENAME = "./Art/Main_Menu.bmp";
const char * AppStateMenu::MUSIC_FILENAME = "./Sound/Music/Main_Menu.ogg";

AppStateMenu::AppStateMenu() {
}

void AppStateMenu::Initialize() {
    background_surf = NULL;//LoadImage(BACKGROUND_FILENAME);
    //background_rect;
    
    MUSIC_STREAM.load(MUSIC_FILENAME);
    MUSIC_STREAM.play();
}

void AppStateMenu::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateMenu::Update() {
}

void AppStateMenu::Draw() {
    SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, background_surf, &background_rect);
    
    SDL_Rect rect = {200, 150, 400, 300};
    Surface::DrawRect(WINDOW, rect, GRAY);
}

void AppStateMenu::Cleanup() {
    MUSIC_STREAM.stop();
}

AppStateBase * AppStateMenu::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateMenu();
    return instance;
}

void AppStateMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
<<<<<<< HEAD
    case SDLK_TAB:  AppStateEvent::New_Event(APPSTATE_NONE);    break;
    default:        AppStateEvent::New_Event(APPSTATE_GAME);    break;
=======
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_GAME);    break;
    default:
        break;
>>>>>>> updated stuff
    }
}

void AppStateMenu::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

#endif
