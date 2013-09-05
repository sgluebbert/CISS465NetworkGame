#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H
 
#include "AppStateBase.h"
//#include "AppStateIntro.h"
//#include "AppStateMenu.h"
//#include "AppStateGame.h"
 
enum AppState {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_MENU,
    APPSTATE_GAME
};
 
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
        static AppState GetActiveAppState();
        static AppStateBase * GetActiveInstance();
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
        //ActiveAppInstance = new AppStateIntro(); || ActiveAppInstance = AppStateIntro::GetInstance();
        break;
    case APPSTATE_MENU:
        //ActiveAppInstance = new AppStateMenu(); || ActiveAppInstance = AppStateMenu::GetInstance();
        break;
    case APPSTATE_GAME:
        //ActiveAppInstance = new AppStateGame(); || ActiveAppInstance = AppStateGame::GetInstance();
        break;
    default:
        //Throw Exception or somehow fallout of the main loop
        break;
    }
 
    if(ActiveAppInstance != NULL)
        ActiveAppInstance->Initialize();
}
 
void AppStateManager::Cleanup() {
    if (ActiveAppInstance != NULL)
        ActiveAppInstance->Cleanup();
        
    delete ActiveAppInstance;
}
 
AppState AppStateManager::GetActiveAppState() {
    return ActiveAppState;
}
 
AppStateBase * AppStateManager::GetActiveInstance() {
    return ActiveAppInstance;
}

#endif
