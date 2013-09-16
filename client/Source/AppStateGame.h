#ifndef APPSTATEGAME_H
#define APPSTATEGAME_H

#include "AppStateBase.h"
<<<<<<< HEAD
=======
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
>>>>>>> updated stuff
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
        
<<<<<<< HEAD
=======
        Entity player;
        
>>>>>>> updated stuff
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
<<<<<<< HEAD
=======

    SurfaceManager * surfaceManager = SurfaceManager::getInstance();
    
    player.SetSurface(surfaceManager->ship01, 64, 64);
    player.max_velocity = 50;
    player.acceleration = 8;
    player.deceleration = 6;
    player.turn_rate = 30;
>>>>>>> updated stuff
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
<<<<<<< HEAD
}

void AppStateGame::Draw() {
    SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, background_surf, &background_rect);
    
    SDL_Rect rect = {200, 150, 400, 300};
    Surface::DrawRect(WINDOW, rect, CYAN);
=======
    Bullet_List::getInstance()->Update();
    player.Update();
}

void AppStateGame::Draw() {
    player.Draw();
    Bullet_List::getInstance()->Draw();
>>>>>>> updated stuff
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
<<<<<<< HEAD
    case SDLK_TAB:  AppStateEvent::New_Event(APPSTATE_NONE);    break;
    default:        AppStateEvent::New_Event(APPSTATE_MENU);    break;
=======
    case SDLK_LEFT:     player.turn_left = true;                    break;
    case SDLK_RIGHT:    player.turn_right = true;                   break;
    case SDLK_UP:       player.move_forward = true;                 break;
    case SDLK_SPACE:    player.TryFire();                           break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        std::cout << "Pos: [" << player.x << ", " << player.y << "]\n";
        std::cout << "Size: [" << player.width << ", " << player.height << "]\n";
        std::cout << "Speed: [" << player.dx << ", " << player.dy << "]\n";
        std::cout << "Velocity: [" << player.velocity << "/" << player.max_velocity << "]\n";
        std::cout << "Throttle: [" << player.throttle << "]\n";
        std::cout << "Angle: [" << player.angle << "]\n";
        break;
>>>>>>> updated stuff
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
<<<<<<< HEAD
=======
    case SDLK_LEFT:     player.turn_left = false;                    break;
    case SDLK_RIGHT:    player.turn_right = false;                   break;
    case SDLK_UP:       player.move_forward = false;                 break;
>>>>>>> updated stuff
        break;
    }
}

#endif
