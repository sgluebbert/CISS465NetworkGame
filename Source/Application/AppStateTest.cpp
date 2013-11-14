#include "AppStateTest.h"
#include "../CollisionManager.h"
#include "../Debug.h"



Star::Star() {
    Drawable::objects.push_back(this);
    drawing_box.x = rand() % 2000;
    drawing_box.y = rand() % 2000;
    drawing_box.w = rand() % 3;
}

void Star::Draw() {
    DrawCircle(drawing_box.x, drawing_box.y, drawing_box.w, true, &WHITE);
}



AppStateBase * AppStateTest::instance = NULL;



AppStateTest::AppStateTest() {
}



void AppStateTest::Initialize() {
    std::cout << "Beginning Initialization..." << std::endl;
    background_texture = surface_manager->background_game;

    std::cout << "Beginning Map Initialization..." << std::endl;
    map = new Map(0);
    map->AddPlanet(0, 0, 0);
    map->AddPlanet(1, 1000, 0);

    player.pawn->team_id = 0;

    std::cout << "Beginning Planet HUD Initialization..." << std::endl;
    planetsHUD = new PlanetsHUD(map->NumPlanets());

    std::cout << "Beginning Math Initialization..." << std::endl;
    srand(time(NULL));
    Initialize_Trig_Table();

    //Just makes sure the instance is initialized before gameplay begins
    std::cout << "Beginning Collision_Manager Initialization..." << std::endl;
    Collision_Manager::Get_Instance();
    
    //sound_manager->Load_Music("./Sound/Music/Battle.ogg");
    //sound_manager->Play_Music();
}

void AppStateTest::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateTest::Update() {
    std::cout << "Beginning Updates..." << std::endl;
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    std::cout << "Beginning Particle Culling..." << Particle::particles.size() << std::endl;
    for (int i = Particle::particles.size() - 1; i >= 0; i--)
        if (Particle::particles[i]->Is_Dead()) {
            for (int j = Rigid_Body::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Rigid_Body::objects[j])
                    Rigid_Body::objects.erase(Rigid_Body::objects.begin() + j);
            for (int j = Drawable::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Drawable::objects[j])
                    Drawable::objects.erase(Drawable::objects.begin() + j);
         delete Particle::particles[i];
            Particle::particles.erase(Particle::particles.begin() + i);
        }

    std::cout << "Beginning Collidable Culling..." << std::endl;
    for (int i = Collidable::objects.size() - 1; i >= 0; i--)
        if (Collidable::objects[i] == NULL)
            Collidable::objects.erase(Collidable::objects.begin() + i);

    std::cout << "Beginning Drawable Culling..." << std::endl;
    for (int i = Drawable::objects.size() - 1; i >= 0; i--)
        if (Drawable::objects[i] == NULL)
            Drawable::objects.erase(Drawable::objects.begin() + i);

    std::cout << "Beginning Rigid Body Culling..." << std::endl;
    for (int i = Rigid_Body::objects.size() - 1; i >= 0; i--)
        if (Rigid_Body::objects[i] == NULL)
            Rigid_Body::objects.erase(Rigid_Body::objects.begin() + i);

    std::cout << "Beginning Rigid Body Updates..." << std::endl;
    for (int i = 0; i < Rigid_Body::objects.size(); i++)
        if (Rigid_Body::objects[i] != NULL)
            Rigid_Body::objects[i]->Update(dt);

    std::cout << "Beginning Player Updates..." << std::endl;
    player.Update(dt);

    //map->Update(dt);
    
    std::cout << "Beginning Planet HUD Updates..." << std::endl;
    planetsHUD->Update(map->planets);

    std::cout << "Beginning Planet Collision Updates..." << std::endl;
    map->PlanetCollision(*player.pawn);

    std::cout << "Beginning Collision Updates..." << std::endl;
    Collision_Manager::Get_Instance()->Update(dt);

    std::cout << "Beginning Camera Updates..." << std::endl;
    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x - viewport.w / 2.0;
    viewport.y = player.pawn->y - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
    std::cout << "End of Updates..." << std::endl;
}

void AppStateTest::Draw() {
    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();

    for (int i = 0; i < Drawable::objects.size(); i++) {
        temp_camera->Map_To_Viewport(Drawable::objects[i]);
        Drawable::objects[i]->Draw();
        temp_camera->Map_To_World(Drawable::objects[i]);
    }

    player.Draw();

    planetsHUD->Draw();
}

void AppStateTest::Cleanup() {
    Drawable::objects.clear();
    Rigid_Body::objects.clear();
    Collidable::objects.clear();

    for (int i = 0; i < Particle::particles.size(); i++)
        delete Particle::particles[i];

    Particle::particles.clear();
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
    case SDLK_p:        player.pawn->team_id ? player.pawn->team_id = 0 : player.pawn->team_id = 1; break;
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