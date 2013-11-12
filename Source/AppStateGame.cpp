#include "AppStateGame.h"



AppStateBase * AppStateGame::instance = NULL;



AppStateGame::AppStateGame() {
    player.offline = false;

    for (int i = 0; i < MaximumClients; ++i)
    {
        players[i] = NULL;
    }
}

void AppStateGame::Initialize() {
    network = NetworkFactory::getInstance();
    network->Init();
    background_texture = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();
    for (int i = 0; i < MaximumClients; ++i)
    {
        if (players[i] == NULL)
            continue;

        players[i]->Update(dt);
    }

    Send();
    Receive();

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x - viewport.w / 2.0;
    viewport.y = player.pawn->y - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
}

void AppStateGame::Draw() {
    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();

    temp_camera->Map_To_Viewport(player.pawn);
    player.Draw();
    temp_camera->Map_To_World(player.pawn);

    for (int i = 0; i < MaximumClients; ++i)
    {
        if (players[i] == NULL || players[i] == player.pawn)
            continue;

        temp_camera->Map_To_Viewport(players[i]);
        players[i]->Draw();
        // std::cout << players[i]->x << ' ' << players[i]->y << '\n';
        temp_camera->Map_To_World(players[i]);
    }
}

void AppStateGame::Send() {
    Parser parser;
    if (!parser.SerializeInput(player.inputs, NUMBER_OF_INPUTS))
        return;
    parser.End();

    network->SendData(parser.GetStream(), player.channel_id);
}

void AppStateGame::Receive() {
    NetString netString;

    while (network->ReceiveData() != -1)
    {
        NetString *temp = network->GetData();
        if (temp != NULL)
            netString += *temp;
    }

    if (netString.BufferLength() == 0)
        return;

    netString.Seek(0);

    bool reading = true;
    while (reading)
    {
        NetworkChunkEnums type;
        unsigned char temp;
        if (!netString.ReadUChar(temp))
            break;

        unsigned char playerId = 0;
        type = (NetworkChunkEnums)temp;

        switch (type)
        {
            case NCE_NEW_PLAYER:
                netString.ReadUChar(playerId);

                if (player.channel_id == -1)
                {
                    player.channel_id = playerId;
                    network->Bind(playerId);
                    players[playerId] = player.pawn;
                }
                break;

            case NCE_PLAYER:
                netString.ReadUChar(playerId);
                Ship *ship;

                // New Player?
                if (players[playerId] == NULL)
                {
                    players[playerId] = new Ship();
                }

                ship = players[playerId];
                ship->Deserialize(&netString);

                bool shipFired[5];
                netString.ReadBool(shipFired[0]);
                netString.ReadBool(shipFired[1]);
                netString.ReadBool(shipFired[2]);
                netString.ReadBool(shipFired[3]);
                netString.ReadBool(shipFired[4]);
                break;

            case NCE_REMOVE_PLAYER:
                netString.ReadUChar(playerId);
                if (players[playerId] != NULL)
                {
                    delete players[playerId];
                    players[playerId] = NULL;
                }

            default:
                reading = false;
        }
    }
}

void AppStateGame::Cleanup() {
    for (int i = 0; i < MaximumClients; ++i)
    {
        if (players[i] != NULL && players[i] != player.pawn)
            delete players[i];
    }
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