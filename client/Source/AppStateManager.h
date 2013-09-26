#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H
 
#include "AppStateIntro.h"
#include "AppStateMenu.h"
#include "AppStateGame.h"
 
class AppStateManager {
    private:
        static AppStateBase * ActiveAppInstance;
        static AppState ActiveAppState;
 
    public:
        static void Initialize();
        static void Events(SDL_Event* Event);
        static void Update();
        static void Draw();
        static void Cleanup();
 
        static void SetActiveAppState(AppState);
};

AppState AppStateManager::ActiveAppState = APPSTATE_NONE;
AppStateBase * AppStateManager::ActiveAppInstance = NULL;
 
void AppStateManager::Initialize() {
    SetActiveAppState(APPSTATE_INTRO);
}
 
void AppStateManager::Events(SDL_Event * Event) {
    if(ActiveAppInstance != NULL)
        ActiveAppInstance->Events(Event);
}
 
void AppStateManager::Update() {
    if(ActiveAppInstance != NULL)
        ActiveAppInstance->Update();
        
    AppStateEvent * appEvent = AppStateEvent::Poll_Event();
    
    if (appEvent != NULL)
        SetActiveAppState(appEvent->Get_Event_Code());
}
 
void AppStateManager::Draw() {
    if(ActiveAppInstance != NULL)
        ActiveAppInstance->Draw();
}
 
void AppStateManager::SetActiveAppState(AppState NewStateID) {
    if (ActiveAppInstance != NULL)
        ActiveAppInstance->Cleanup();
    
    ActiveAppState = NewStateID;
    
    switch(ActiveAppState) {
    case APPSTATE_INTRO:
        ActiveAppInstance = AppStateIntro::GetInstance();
        break;
    case APPSTATE_MENU:
        ActiveAppInstance = AppStateMenu::GetInstance();
        break;
    case APPSTATE_GAME:
        ActiveAppInstance = AppStateGame::GetInstance();
        break;
    default:
        delete ActiveAppInstance;
        ActiveAppInstance = NULL;
        
        break;
    }
 
    if(ActiveAppInstance != NULL)
        ActiveAppInstance->Initialize();
}
 
void AppStateManager::Cleanup() {
    if (ActiveAppInstance != NULL)
        ActiveAppInstance->Cleanup();
        
    delete ActiveAppInstance;
    ActiveAppInstance = NULL;
}

#endif
