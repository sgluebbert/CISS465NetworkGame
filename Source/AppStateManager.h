#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H
 


#include "ApplicationEvent.h"
#include "AppStateIntro.h"
#include "AppStateMenu.h"
#include "AppStateGame.h"
 
 
 
class AppStateManager {
    private:
        static AppStateBase * instance;
        static AppState active_state;
 
    public:
        static void Initialize();
        static void Events(SDL_Event* Event);
        static void Update();
        static void Draw();
        static void Cleanup();
 
        static void Set_Active_State(AppState);
};



AppState AppStateManager::active_state = APPSTATE_NONE;
AppStateBase * AppStateManager::instance = NULL;
 
 
 
void AppStateManager::Initialize() {
    Set_Active_State(APPSTATE_INTRO);
}
 
void AppStateManager::Events(SDL_Event * Event) {
    if(instance != NULL)
        instance->Events(Event);
}
 
void AppStateManager::Update() {
    if(instance != NULL)
        instance->Update();
        
    AppStateEvent * appEvent = AppStateEvent::Poll_Event();
    
    if (appEvent != NULL)
        Set_Active_State(appEvent->Get_Event_Code());
}
 
void AppStateManager::Draw() {
    if(instance != NULL)
        instance->Draw();
}
 
void AppStateManager::Set_Active_State(AppState NewStateID) {
    if (instance != NULL)
        instance->Cleanup();
    
    active_state = NewStateID;
    
    switch(active_state) {
    case APPSTATE_INTRO:
        instance = AppStateIntro::GetInstance();
        break;
    case APPSTATE_MENU:
        instance = AppStateMenu::GetInstance();
        break;
    case APPSTATE_GAME:
        instance = AppStateGame::GetInstance();
        break;
    default:
        delete instance;
        instance = NULL;
        Application_Event::New_Event(EXIT_FLAG);
        
        break;
    }
 
    if(instance != NULL)
        instance->Initialize();
}
 
void AppStateManager::Cleanup() {
    if (instance != NULL)
        instance->Cleanup();
        
    delete instance;
    instance = NULL;
}



#endif
