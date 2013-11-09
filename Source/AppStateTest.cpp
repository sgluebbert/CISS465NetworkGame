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
    //sound_manager->Stop_Music();
}



AppStateBase * AppStateTest::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateTest();
    return instance;
}



void AppStateTest::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = true;                break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = true;               break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = true;             break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = true;            break;
    /*Change Weapon Firing to use the PRESSED_KEY array rather than the toggles*/
    case SDLK_1:        player.inputs[FIRE_ENERGY] = true;              break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = true;           break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = true;           break;
    case SDLK_4:        player.inputs[FIRE_MINE] = true;                break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = true;             break;
    case SDLK_d:        player.pawn->Damage_Shields(10.0);              break;
    case SDLK_f:        player.pawn->Damage_Armor(10.0);                break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);        break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);        break;
    default:
        break;
    }
}

void AppStateTest::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = false;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = false;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = false;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = false;           break;
    /*Change Weapon Firing to use the PRESSED_KEY array rather than the toggles*/
    case SDLK_1:        player.inputs[FIRE_ENERGY] = false;             break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = false;          break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = false;          break;
    case SDLK_4:        player.inputs[FIRE_MINE] = false;               break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = false;            break;
    default:
        break;
    }
}