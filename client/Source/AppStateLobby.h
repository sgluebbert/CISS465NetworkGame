#ifndef APPSTATELOBBY_H
#define APPSTATELOBBY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		
public:
		AppStateLobby();
 
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

AppStateLobby::AppStateLobby() {
}

void AppStateLobby::Initialize() { 
    //SurfaceManager * surfaceManager = SurfaceManager::getInstance();
    background_surf = NULL;//surfaceManager->background_main_menu01;
    //background_rect;
}

void AppStateLobby::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateLobby::Update() {
}

void AppStateLobby::Draw() {
}

void AppStateLobby::Cleanup() {
}

AppStateBase * AppStateLobby::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateLobby();
    return instance;
}

void AppStateLobby::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
	//ASCII Keys need to be registered and the mod will need to be used to
	//allow for use of capitals and such.
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:            break;
    }
}

void AppStateLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

#endif
