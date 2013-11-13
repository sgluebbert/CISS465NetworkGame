#ifndef APPSTATEMENU_H
#define APPSTATEMENU_H



#include "AppStateBase.h"
#include "../Menu.h"
#include "../System.h"
 


class AppStateMenu : public AppStateBase {
private:
        static AppStateBase * instance;
        
        Texture* background_surf;
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



#endif
