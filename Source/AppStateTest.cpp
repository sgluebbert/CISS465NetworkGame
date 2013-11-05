#include "AppStateTest.h"



AppStateBase * AppStateTest::instance = NULL;



AppStateTest::AppStateTest() {
}



void AppStateTest::Initialize() {
    background_texture = surface_manager->background_game;
    
    // sound_manager->Load_Music("./Sound/Music/Battle.ogg");
    // sound_manager->Play_Music();
}

void AppStateTest::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateTest::Update() {
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    player.Update(dt);

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x + player.pawn->w / 2.0 - viewport.w / 2.0;
    viewport.y = player.pawn->y + player.pawn->w / 2.0 - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
}

void AppStateTest::Draw() {
    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();

    background_texture->DrawAtRect(temp_rect.x, temp_rect.y, temp_rect.w, temp_rect.h);

    player.pawn->Map_To_Viewport(temp_rect);
    player.Draw();
    player.pawn->Map_To_World(temp_rect);
}

void AppStateTest::Cleanup() {
    sound_manager->Stop_Music();
}



AppStateBase * AppStateTest::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateTest();
    return instance;
}



void AppStateTest::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = 1;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = 1;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = 1;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = 1;           break;
    case SDLK_d:        player.pawn->health = 0.25 * player.pawn->max_health;           break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        break;
    }
}

void AppStateTest::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = 0;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = 0;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = 0;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = 0;           break;
    default:
        break;
    }
}