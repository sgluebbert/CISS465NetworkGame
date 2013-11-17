#include "AppStateManager.h"



AppState AppStateManager::active_state = APPSTATE_NONE;
AppStateBase * AppStateManager::instance = NULL;
 
 
 
void AppStateManager::Initialize(bool gui) {
    if (gui)
        Set_Active_State(APPSTATE_INTRO);
    else
        Set_Active_State(APPSTATE_GAME_SERVER);
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
    case APPSTATE_LOBBY:
        instance = AppStateLobby::GetInstance();
        break;
    case APPSTATE_GAME:
        instance = AppStateGame::GetInstance();
        break;
    case APPSTATE_GAME_SERVER:
        instance = AppStateGameServer::GetInstance();
        break;
    case APPSTATE_TEST:
        instance = AppStateTest::GetInstance();
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