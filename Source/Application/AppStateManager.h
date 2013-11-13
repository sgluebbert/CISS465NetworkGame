#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H
 


#include "ApplicationEvent.h"
#include "AppStateIntro.h"
#include "AppStateMenu.h"
#include "AppStateGame.h"
#include "AppStateGameServer.h"
#include "AppStateTest.h"
 
 
 
class AppStateManager {
    private:
        static AppStateBase * instance;
        static AppState active_state;
 
    public:
        static void Initialize(bool);
        static void Events(SDL_Event* Event);
        static void Update();
        static void Draw();
        static void Cleanup();
 
        static void Set_Active_State(AppState);
};



#endif
