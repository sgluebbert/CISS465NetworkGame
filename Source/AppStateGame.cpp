#include "AppStateGame.h"



AppStateBase * AppStateGame::instance = NULL;



AppStateGame::AppStateGame() {
}

void AppStateGame::Initialize() {
    background_texture = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();
    player.Update(dt);

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x + player.pawn->w / 2.0 - viewport.w / 2.0;
    viewport.y = player.pawn->y + player.pawn->w / 2.0 - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
}

void AppStateGame::Draw() {
    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();
    player.pawn->Map_To_Viewport(temp_rect);
    player.Draw();
    player.pawn->Map_To_World(temp_rect);
}

void AppStateGame::Cleanup() {
    sound_manager->Stop_Music();
}

AppStateBase * AppStateGame::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGame();
    return instance;
}

void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = true;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = true;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = true;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = true;           break;
    case SDLK_d:        player.pawn->health = 0.25 * player.pawn->max_health;           break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = false;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = false;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = false;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = false;           break;
    default:
        break;
    }
}