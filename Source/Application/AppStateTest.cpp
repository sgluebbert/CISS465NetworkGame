#include "AppStateTest.h"



Star::Star() {
    bounding_volume.x = rand() % 2000;
    bounding_volume.y = rand() % 2000;
    bounding_volume.r = rand() % 3;
}

void Star::Draw() {
    DrawCircle(bounding_volume.x, bounding_volume.y, bounding_volume.r, true, &WHITE);
}



AppStateBase * AppStateTest::instance = NULL;



AppStateTest::AppStateTest() {
}



void AppStateTest::Initialize() {
    background_texture = surface_manager->background_game;
    map = new Map(0);

    srand(time(NULL));
    Initialize_Trig_Table();

    for (int i = 0; i < 500; i++)
        stars.push_back(Star());
    
    //sound_manager->Load_Music("./Sound/Music/Battle.ogg");
    //sound_manager->Play_Music();
}

void AppStateTest::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateTest::Update() {
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    player.Update(dt);

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x - viewport.w / 2.0;
    viewport.y = player.pawn->y - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
}

void AppStateTest::Draw() {
    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();

    for (int i = 0; i < stars.size(); i++) {
        temp_camera->Map_To_Viewport(&stars[i].bounding_volume);
        if (!(stars[i].bounding_volume.x < 0 || stars[i].bounding_volume.x > 800 ||
              stars[i].bounding_volume.y < 0 || stars[i].bounding_volume.y > 600))
            stars[i].Draw();
        temp_camera->Map_To_World(&stars[i].bounding_volume);
    }

    temp_camera->Map_To_Viewport(player.pawn);
    player.Draw();
    temp_camera->Map_To_World(player.pawn);
}

void AppStateTest::Cleanup() {
    delete map;
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
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = true;                            break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = true;                           break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = true;                         break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = true;                        break;
    case SDLK_1:        player.inputs[FIRE_ENERGY] = true;                          break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = true;                       break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = true;                       break;
    case SDLK_4:        player.inputs[FIRE_MINE] = true;                            break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = true;                         break;
    case SDLK_d:        player.pawn->Damage_Shields(10.0);                          break;
    case SDLK_f:        player.pawn->Damage_Armor(10.0);                            break;
    case SDLK_t:        player.info_feed.Player_Joined("Quicksilver");              break;
    case SDLK_y:        player.info_feed.Player_Disconnected("Quicksilver");        break;
    case SDLK_u:        player.info_feed.Player_Died("Quicksilver");                break;
    case SDLK_i:        player.info_feed.Player_Killed("Quicksilver", "Blah");      break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);                    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);                    break;
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