#ifndef APPSTATETEST_H
#define APPSTATETEST_H



#include "AppStateBase.h"
#include "../Camera.h"
#include "../Client.h"
#include "../Clock.h"
#include "../Map.h"
#include "../System.h"
#include "../Planet.h"
#include "../HUD/HUDManager.h"

class AppStateTest : public AppStateBase {
private:
    static AppStateBase * instance;
    AppStateTest();
        
    Texture * background_texture;
    Rect<double> background_rect;
        
    Client player;
    Map * map;
public:
 
    void Initialize();
    void Events(SDL_Event *);
    void Update();
    void Draw();
    void Cleanup();
        
    static AppStateBase * GetInstance();
 
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void createHUD();
};



#endif
