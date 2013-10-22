#ifndef APPSTATEMENU_H
#define APPSTATEMENU_H



#include "AppStateBase.h"
#include "Menu.h"
#include "System.h"
 


class AppStateMenu : public AppStateBase {
private:
        static AppStateBase * instance;
        
        GLuint * background_surf;
        SDL_Rect background_rect;
        
        Menu main_menu;
        
        static const char * MUSIC_FILENAME;
        
        AppStateMenu();
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



AppStateBase * AppStateMenu::instance = NULL;

const char * AppStateMenu::MUSIC_FILENAME = "./Sound/Music/Main_Menu.ogg";



AppStateMenu::AppStateMenu() {
}

void AppStateMenu::Initialize() {
    background_surf = &surface_manager->background_main_menu;
    //background_rect;
    
    sound_manager->Load_Music(MUSIC_FILENAME);
    sound_manager->Play_Music();
    
    main_menu.Add_Option("Play");
    main_menu.Add_Option("Exit");
    
    main_menu.Center_To_Window();
}

void AppStateMenu::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateMenu::Update() {
}

void AppStateMenu::Draw() {
    DrawImageRect(*background_surf, 0, 0, WINDOW_BOUNDING_BOX.w + 400, WINDOW_BOUNDING_BOX.h + 400);
    main_menu.Draw();
}

void AppStateMenu::Cleanup() {
    sound_manager->Stop_Music();
}

AppStateBase * AppStateMenu::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateMenu();
    return instance;
}

void AppStateMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_UP:       main_menu.Move_Previous();                  break;
    case SDLK_DOWN:     main_menu.Move_Next();                      break;
    case SDLK_RETURN:
        switch(main_menu.Select()) {
        case 0:     AppStateEvent::New_Event(APPSTATE_GAME);            break;
        default:    AppStateEvent::New_Event(APPSTATE_NONE);            break;
        }
        break;
    default:
        break;
    }
}

void AppStateMenu::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}



#endif
