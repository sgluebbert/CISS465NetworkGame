#include "AppStateTest.h"
#include "../CollisionManager.h"
#include "../Debug.h"
#include "../SoundManager.h"



AppStateBase * AppStateTest::instance = NULL;



AppStateTest::AppStateTest() {
}



void AppStateTest::Initialize() {
    std::cout << "Beginning Initialization..." << std::endl;

    Sound_Manager * music = Sound_Manager::Get_Instance();
    music->Load_Music("Sound/Music/Battle.ogg");
    music->Play_Music();

    //Just makes sure the instance is initialized before gameplay begins
    std::cout << "Beginning Collision_Manager Initialization..." << std::endl;
    Collision_Manager::Get_Instance();

    //background_texture = surface_manager->background_game;

    std::cout << "Beginning Math Initialization..." << std::endl;
    srand(time(NULL));
    Initialize_Trig_Table();

    std::cout << "Beginning Map Initialization..." << std::endl;
    map = new Map(rand(), 0.5);
    map->Generate_Map();

    std::cout << "Beginning Ship Initialization..." << std::endl;
    Ship::Initialize_Ships(map->max_players_per_team * 2);
    int pawn_index = Ship::Add_Ship(BLUE_TEAM, INTERCEPTOR, 100, 100, 5.0);

    if (pawn_index >= 0)
        player.pawn = Ship::ships[pawn_index];

    Print_Collidables();
    Print_Drawables();
    Print_Rigid_Bodies();
}

void AppStateTest::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateTest::Update() {
    // std::cout << "Beginning Updates..." << std::endl;
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    // std::cout << "Beginning Particle Culling..." << std::endl;
    for (int i = Particle::particles.size() - 1; i >= 0; i--)
        if (Particle::particles[i]->Is_Dead()) {
            for (int j = Rigid_Body::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Rigid_Body::objects[j])
                    Rigid_Body::objects.erase(Rigid_Body::objects.begin() + j);
            for (int j = Drawable::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Drawable::objects[j])
                    Drawable::objects.erase(Drawable::objects.begin() + j);
            for (int j = Collidable::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Collidable::objects[j])
                    Collidable::objects.erase(Collidable::objects.begin() + j);

            delete Particle::particles[i];
            Particle::particles.erase(Particle::particles.begin() + i);
        }

    // std::cout << "Beginning Collidable Culling..." << std::endl;
    for (int i = Collidable::objects.size() - 1; i >= 0; i--)
        if (Collidable::objects[i] == NULL)
            Collidable::objects.erase(Collidable::objects.begin() + i);

    // std::cout << "Beginning Drawable Culling..." << std::endl;
    for (int i = Drawable::objects.size() - 1; i >= 0; i--)
        if (Drawable::objects[i] == NULL)
            Drawable::objects.erase(Drawable::objects.begin() + i);

    // std::cout << "Beginning Rigid Body Culling..." << std::endl;
    for (int i = Rigid_Body::objects.size() - 1; i >= 0; i--)
        if (Rigid_Body::objects[i] == NULL)
            Rigid_Body::objects.erase(Rigid_Body::objects.begin() + i);

    // std::cout << "Beginning Rigid Body Updates..." << std::endl;
    for (int i = 0; i < Rigid_Body::objects.size(); i++) {
        Rigid_Body::objects[i]->Update(dt);
    }

    // std::cout << "Beginning Player Updates..." << std::endl;
    player.Update(dt);

    // std::cout << "Beginning Collision Updates..." << std::endl;
    Collision_Manager::Get_Instance()->Update(dt);

    // std::cout << "Beginning Camera Updates..." << std::endl;
    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x - viewport.w / 2.0;
    viewport.y = player.pawn->y - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
    // std::cout << "End of Updates..." << std::endl;
}

void AppStateTest::Draw() {
    Camera * temp_camera = Camera::getInstance();

    for (int i = 0; i < Drawable::objects.size(); i++) {
        temp_camera->Map_To_Viewport(Drawable::objects[i]);
        Drawable::objects[i]->Draw();
        temp_camera->Map_To_World(Drawable::objects[i]);
    }

    player.Draw();
}

void AppStateTest::Cleanup() {
    for (int i = 0; i < Particle::particles.size(); i++)
        if (Particle::particles[i] != NULL)
            delete Particle::particles[i];

    Ship::Cleanup_Ships();


    Sound_Manager * music = Sound_Manager::Get_Instance();
    music->Stop_Music();

    Particle::particles.clear();

    Drawable::objects.clear();
    Rigid_Body::objects.clear();
    Collidable::objects.clear();

    delete map;
}



AppStateBase * AppStateTest::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateTest();
    return instance;
}



void AppStateTest::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = true;                                                                            break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = true;                                                                           break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = true;                                                                         break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = true;                                                                        break;
    case SDLK_1:        player.inputs[FIRE_ENERGY] = true;                                                                          break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = true;                                                                       break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = true;                                                                       break;
    case SDLK_4:        player.inputs[FIRE_MINE] = true;                                                                            break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = true;                                                                         break;
    case SDLK_d:        player.pawn->Damage_Shields(10.0);                                                                          break;
    case SDLK_f:        player.pawn->Damage_Armor(10.0);                                                                            break;
    case SDLK_t:        player.info_feed.Player_Joined("Quicksilver");                                                              break;
    case SDLK_y:        player.info_feed.Player_Disconnected("Quicksilver");                                                        break;
    case SDLK_u:        player.info_feed.Player_Died("Quicksilver");                                                                break;
    case SDLK_i:        player.info_feed.Player_Killed("Quicksilver", "Blah");                                                      break;
    case SDLK_o:        std::cout << "Memory Usage: " << Get_Memory_Usage() << "kb" << std::endl;                                   break;
    case SDLK_c:        Print_Collidables();                                                                                        break;
    case SDLK_v:        Print_Drawables();                                                                                          break;
    case SDLK_b:        Print_Rigid_Bodies();                                                                                       break;
    case SDLK_n:        Print_Planets();                                                                                            break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);                                                                    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);                                                                    break;
    case SDLK_p:        player.pawn->team_id == RED_TEAM ? player.pawn->team_id = BLUE_TEAM : player.pawn->team_id = RED_TEAM;      break;
    default:                                                                                                                        break;
    }
}

void AppStateTest::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = false;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = false;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = false;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = false;           break;
    case SDLK_1:        player.inputs[FIRE_ENERGY] = false;             break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = false;          break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = false;          break;
    case SDLK_4:        player.inputs[FIRE_MINE] = false;               break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = false;            break;
    default:
        break;
    }
}