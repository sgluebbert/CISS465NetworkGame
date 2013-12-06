#include "AppStateGameServer.h"


AppStateBase * AppStateGameServer::instance = NULL;



AppStateGameServer::AppStateGameServer(DebugLevel l)
	: debugLevel(l), clientCount(0), secondsToStart(1), inLobby(true), teamRedCount(0), teamBlueCount(0), state(GSE_WAITING)
{
	tryMainAgain = 0;

	time(&secondsToStartLastTick);
	for (int i = 0; i < MaximumClients; i++)
		clients[i] = NULL;
	availablePlayerIds.set();

	winner = NO_TEAM;
}

void AppStateGameServer::Initialize() {
	srand(time(NULL));
	map = new Map(rand(), 1.0);

	//////////////////////////////////////////////////////////////////////////////////
	// Server stuff
	lobbyName = std::string(lobbySetName);

	if (lobbySetPort == 0)
	{
		networkGame = NetworkFactory::getInstance("./conf/networkLobby.conf");
		if (networkGame->Init(true) == -1)
			exit(1);
	}
	else
	{
		networkGame = NetworkFactory::getInstance(TCP, "localhost", lobbySetPort);
		if (networkGame->Init(true) == -1)
			exit(1);
	}

	networkMainServer = NetworkFactory::getInstance("./conf/networkMainServer.conf");
	if (networkMainServer->Init() != 1)
		tryMainAgain = 100;
	else
		UpdateMainServer();

	time(&lastActivity);
	time(&lastSpeedDisplay);
	std::cout << "[ Server Started ] " << CurrentDateTime() << std::endl;
	//////////////////////////////////////////////////////////////////////////////////

    Collision_Manager::Get_Instance();
    Initialize_Trig_Table();
}

void AppStateGameServer::SwitchToGameMode() {
	// Switch to regular game mode configurations
	for (int i = 0; i < MaximumClients; i++)
	{
		if (clients[i] == NULL)
			continue;
		time(&clients[i]->last_input);
		expectedClients.push_back(clients[i]);
		clients[i] = NULL;
	}

	clientCount = 0;
	networkGame->Close();
	delete networkGame;

	if (lobbySetPort == 0)
	{
		networkGame = NetworkFactory::getInstance();
		networkGame->Init(true);
	}
	else
	{
		networkGame = NetworkFactory::getInstance(UDP, "localhost", lobbySetPort);
		networkGame->Init(true);
	}

	state = GSE_GAME_COUNTDOWN;
	inLobby = false;
	secondsToStart = 15;
	time(&secondsToStartLastTick);
	time(&lastActivity);

	std::cout << "[ Server Switching ] " << CurrentDateTime() << " >>> Switching to game mode. Map Seed: " << map->SEED << "." << std::endl;

	map->Generate_Map();
	Ship::Initialize_Ships(map->max_players_per_team * 2);
}

void AppStateGameServer::Events(SDL_Event * Event) {
	OnEvent(Event);
}

void AppStateGameServer::Update() {
	// Insert game logic in UpdateGame()

	//////////////////////////////////////////////////////////////////////////////////
	// Server Stuff
	time_t now;
	time(&now);

	if (debugLevel > DL_LOW)
	{
		if (now - lastSpeedDisplay >= 1)
		{
			time(&lastSpeedDisplay);
			if (clientCount > 0)
				std::cout << "[ Server Speed ] " << CurrentDateTime() << " >>> Clients: " << clientCount << "; Frames Per Second: " << Clock::Frame_Control.Get_FPS() << std::endl;
		}
	}

	// Have we contacted the main server yet?
	if (!networkMainServer->IsInited())
	{
		if (tryMainAgain-- <= 0)
		{
			// std::cout << "Trying main again\n";
			if (networkMainServer->Init() != 1)
				tryMainAgain = 100;
			else
				UpdateMainServer();
		}
	}
	else
	{
		int receiveId = networkMainServer->ReceiveData();
		if (receiveId == -2) // Server hung up
		{
			std::cout << "MainServer hung up.\n";
			networkMainServer->Close();
			tryMainAgain = 100;
		}
	}

	if (inLobby)
		UpdateLobby();
	else
		UpdateGame();

	if (now - lastActivity > TimeoutInSeconds && clientCount == 0)
	{
		std::cout << "[ Server Timed Out ] " << CurrentDateTime() << " >>> No activity for " << TimeoutInSeconds << " seconds." << std::endl;

		AppStateEvent::New_Event(APPSTATE_NONE);
	}
	//////////////////////////////////////////////////////////////////////////////////
}

void AppStateGameServer::UpdateMainServer()
{
	if (!networkMainServer->IsInited())
		return;

	NetString string;
	string.WriteUChar(NCE_LOBBY);
	string.WriteString(lobbyName);
	Uint16 encodedPort = networkGame->GetRecvPort();
	int port = SDLNet_Read16(&encodedPort);
	string.WriteInt(port);
	string.WriteUChar(state);
	string.WriteUChar((unsigned char)clientCount);
	string.WriteInt(map->SEED);
	string.WriteFloat(map->map_scale);
	string.WriteUChar(NCE_END);
	networkMainServer->SendData(&string, 0);
}

void AppStateGameServer::UpdateLobby() {

	HandleLobbyConnections();

	if (state == GSE_WAITING && teamRedCount >= map->min_players_per_team && teamBlueCount >= map->min_players_per_team)
	{
		secondsToStart = 15;
		state = GSE_LOBBY_COUNTDOWN;
		SendStateUpdate();
	}

	if (state == GSE_LOBBY_COUNTDOWN && (teamRedCount < map->min_players_per_team || teamBlueCount < map->min_players_per_team))
	{
		state = GSE_WAITING;
		SendStateUpdate();
	}

	if (state == GSE_LOBBY_COUNTDOWN)
	{
		time_t now;
		time(&now);
		if (now - secondsToStartLastTick >= 1)
		{
			time(&secondsToStartLastTick);
			secondsToStart--;

			if (secondsToStart == 0)
			{
				state = GSE_TRANSITION;
				SendStateUpdate();
			}

			if (state == GSE_TRANSITION)
			{
				SwitchToGameMode();
				SendStateUpdate();
			}
		}
	}
}

void AppStateGameServer::HandleLobbyConnections() {
	// I will assume lobby is TCP
	NetString string;
	Client *client = NULL;
	while (true)
	{
		std::vector<int> newClients, removedClients;
		int receiveId = networkGame->ReceiveData(&newClients, &removedClients);

		// Handle new connections
		for (std::vector<int>::iterator it = newClients.begin(); it != newClients.end(); it++)
		{
			if (clients[*it] != NULL)
			{
				availablePlayerIds[clients[*it]->player_id] = true;
				delete clients[*it];
				clientCount--;
				clients[*it] = NULL;
			}

			if (teamRedCount >= map->max_players_per_team && teamBlueCount >= map->max_players_per_team)
			{
				NetString netString;
				netString.WriteUChar(NCE_TOO_MANY_PLAYERS);
				netString.WriteUChar(NCE_END);
				networkGame->SendData(&netString, *it);
				networkGame->RemoveConnection(*it);
			}
			else
			{
				client = new Client();
				client->offline = false;
				client->channel_id = *it;
				clientCount++;
				clients[*it] = client;

				SendStateUpdate(*it);

				if (debugLevel > DL_NONE)
					std::cout << "[ New Client Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
			}

			time(&lastActivity);
		}

		for (std::vector<int>::iterator it = removedClients.begin(); it != removedClients.end(); it++)
		{
			if (clients[*it] == NULL)
				continue;

			availablePlayerIds[clients[*it]->player_id] = true;
			delete clients[*it];
			clients[*it] = NULL;
			clientCount--;

			// Update everyone of potential new information
			MakeTeamsEven();
			SendLobbyPlayersToAll();

			if (debugLevel > DL_NONE)
				std::cout << "[ Client Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;

			time(&lastActivity);
		}

		// Did we get a message ready to read?
		if (receiveId == -1)
			break;

		time(&lastActivity);

		// Read that new message
		client = clients[receiveId];
		NetString *netString = networkGame->GetData(receiveId);

		bool reading = true, shouldSyncPlayers = false;
		while (reading)
		{
			NetworkChunkEnums type;
			unsigned char temp;
			if (!netString->ReadUChar(temp))
				break;

			unsigned char channelId = 0;
			type = (NetworkChunkEnums)temp;

			switch (type)
			{
				case NCE_PLAYER:
					netString->ReadString(client->player_name);

					bool playerExists = false;
					for (int i = 0; i < MaximumClients; i++)
					{
						if (clients[i] == NULL || i == receiveId)
							continue;

						if (clients[i]->player_name == client->player_name)
						{
							playerExists = true;
							break;
						}
					}

					if (playerExists)
					{
						NetString response;
						response.WriteUChar(NCE_ALREADY_JOINED);
						response.WriteUChar(NCE_END);
						networkGame->SendData(&response, receiveId);
						delete client;
						clients[receiveId] = NULL;
						clientCount--;
					}
					else
					{
						std::cout << "Got " << client->player_name << '\n';
						client->player_id = GetNextPlayerId();
						MakeTeamsEven();

						shouldSyncPlayers = true;
					}
					break;
			}
		}

		delete netString;

		if (shouldSyncPlayers)
			SendLobbyPlayersToAll();
	}
}

unsigned char AppStateGameServer::GetNextPlayerId()
{
	for (int i = 0; i < MaximumClients; i++)
	{
		if (availablePlayerIds[i])
		{
			availablePlayerIds[i] = false;
			return i;
		}
	}
}

void AppStateGameServer::SendLobbyPlayersToAll()
{
	NetString string;
	string.WriteUChar(NCE_MAP);
	string.WriteInt(map->SEED);
	string.WriteInt(map->map_scale);
	string.WriteUChar(NCE_END);

	string.WriteUChar(NCE_LOBBY_PLAYER_SYNC);

	for (int i = 0; i < MaximumClients; i++)
	{
		if (clients[i] == NULL)
			continue;
		string.WriteUChar(NCE_PLAYER);
		string.WriteUChar(i);
		string.WriteUChar(clients[i]->team_id);
		string.WriteInt(clients[i]->player_id);
		string.WriteString(clients[i]->player_name);
	}

	string.WriteUChar(NCE_END);
	SendToAll(&string);
	UpdateMainServer();
}

bool AppStateGameServer::MakeTeamsEven(int id) {
	teamRedCount = 0;
	teamBlueCount = 0;

	if (inLobby)
	{
		for (int i = 0; i < MaximumClients; i++)
		{
			if (clients[i] == NULL)
				continue;

			if (teamRedCount > teamBlueCount)
			{
				clients[i]->team_id = BLUE_TEAM;
				teamBlueCount++;
			}
			else
			{
				clients[i]->team_id = RED_TEAM;
				teamRedCount++;
			}
		}
	}
	else
	{
		if (id < 0 || clients[id] == NULL) // shouldn't happen
			return false;

		// count up existing players.
		for (int i = 0; i < MaximumClients; i++)
		{
			if (clients[i] == NULL || i == id)
				continue;

			if (clients[i]->team_id == RED_TEAM)
				teamRedCount++;
			else
				teamBlueCount++;

			if (clients[i]->pawn != NULL)
				clients[i]->pawn->team_id = clients[i]->team_id;
		}

		// count up expected players
		for (std::vector<Client *>::iterator it = expectedClients.begin(); it != expectedClients.end(); it++)
		{
			if ((*it)->team_id == RED_TEAM)
				teamRedCount++;
			else
				teamBlueCount++;
		}

		if (teamRedCount >= map->max_players_per_team && teamBlueCount >= map->max_players_per_team)
			return false;

		if (teamRedCount > teamBlueCount)
		{
			clients[id]->team_id = BLUE_TEAM;
			if (clients[id]->pawn != NULL)
				clients[id]->pawn->team_id = clients[id]->team_id;
			teamBlueCount++;
		}
		else
		{
			clients[id]->team_id = RED_TEAM;
			if (clients[id]->pawn != NULL)
				clients[id]->pawn->team_id = clients[id]->team_id;
			teamRedCount++;
		}
	}

	return true;
}

void AppStateGameServer::SendStateUpdate(int id) {
	NetString string;
	string.WriteUChar(NCE_LOBBY_STATE_SYNC);
	string.WriteUChar(state);
	
	if (state == GSE_LOBBY_COUNTDOWN || state == GSE_GAME_COUNTDOWN)
	{
		string.WriteUChar(secondsToStart);
		string.WriteInt(secondsToStartLastTick);
	}
	else if (state == GSE_GAME_ENDED)
		string.WriteUChar(winner);

	string.WriteUChar(NCE_END);

	if (id == -1)
		SendToAll(&string);
	else
		networkGame->SendData(&string, id);
	UpdateMainServer();
}

void AppStateGameServer::UpdateGame() {

	//////////////////////////////////////////////////////////////////////////////////
	// Server Stuff

	// Handle new/removed connections and their input
	HandleGameConnections();

	//////////////////////////////////////////////////////////////////////////////////
	// Game Logic
	double dt = Clock::Frame_Control.Get_Time_Per_Frame();

	if (state == GSE_GAME)
	{
		for (int i = 0; i < MaximumClients; i++)
		{
			if (clients[i] == NULL)
				continue;
			clients[i]->Update(dt);
		}
	}
	else if (state == GSE_GAME_COUNTDOWN)
	{
		time_t now;
		time(&now);
		if (now - secondsToStartLastTick >= 1)
		{
			time(&secondsToStartLastTick);
			secondsToStart--;
			SendStateUpdate();

			if (secondsToStart == 0)
			{
				state = GSE_GAME;
				SendStateUpdate();
			}
		}
	}


	// std::cout << "Beginning Particle Culling..." << std::endl;
	for (int i = Particle::particles.size() - 1; i >= 0; i--)
	{
		if (Particle::particles[i]->Is_Dead())
		{
			for (int j = Rigid_Body::objects.size() - 1; j >= 0; j--)
			{
				if (Particle::particles[i] == Rigid_Body::objects[j])
					Rigid_Body::objects.erase(Rigid_Body::objects.begin() + j);
			}

			for (int j = Drawable::objects.size() - 1; j >= 0; j--)
			{
				if (Particle::particles[i] == Drawable::objects[j])
					Drawable::objects.erase(Drawable::objects.begin() + j);
			}

			for (int j = Collidable::objects.size() - 1; j >= 0; j--)
			{
				if (Particle::particles[i] == Collidable::objects[j])
					Collidable::objects.erase(Collidable::objects.begin() + j);
			}

			delete Particle::particles[i];
			Particle::particles.erase(Particle::particles.begin() + i);
		}
	}

	// std::cout << "Beginning Collidable Culling..." << std::endl;
	for (int i = Collidable::objects.size() - 1; i >= 0; i--)
	{
		if (Collidable::objects[i] == NULL)
			Collidable::objects.erase(Collidable::objects.begin() + i);
	}

	// std::cout << "Beginning Drawable Culling..." << std::endl;
	for (int i = Drawable::objects.size() - 1; i >= 0; i--)
	{
		if (Drawable::objects[i] == NULL)
			Drawable::objects.erase(Drawable::objects.begin() + i);
	}

	// std::cout << "Beginning Rigid Body Culling..." << std::endl;
	for (int i = Rigid_Body::objects.size() - 1; i >= 0; i--)
	{
		if (Rigid_Body::objects[i] == NULL)
			Rigid_Body::objects.erase(Rigid_Body::objects.begin() + i);
	}

	// std::cout << "Beginning Rigid Body Updates..." << std::endl;
	for (int i = 0; i < Rigid_Body::objects.size(); i++)
		Rigid_Body::objects[i]->Update(dt);

    Collision_Manager::Get_Instance()->Update(dt);

	if (state == GSE_GAME) // contrary to game countdown or game ended
	{
		if (teamRedCount < map->min_players_per_team)
		{
			// Team blue won
			std::cout << "Team blue Won!\n";
			state = GSE_GAME_ENDED;
			winner = BLUE_TEAM;
			SendStateUpdate();
			AppStateEvent::New_Event(APPSTATE_NONE);
			return;
		}
		else if (teamBlueCount < map->min_players_per_team)
		{
			// Team red won
			std::cout << "Team red Won!\n";
			state = GSE_GAME_ENDED;
			winner = RED_TEAM;
			SendStateUpdate();
			AppStateEvent::New_Event(APPSTATE_NONE);
			return;
		}

		// std::cout << Planet::planet_graph.front()->team_id << "  " << Planet::planet_graph.back()->team_id << '\n';
		Team planetsWinner = Planet::Win_Condition();
		if (planetsWinner != NO_TEAM)
		{
			std::cout << "Team " << planetsWinner << " Won!\n";
			state = GSE_GAME_ENDED;
			winner = planetsWinner;
			SendStateUpdate();
			AppStateEvent::New_Event(APPSTATE_NONE);
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Server Stuff

	// Update all of the connections to the game state
	UpdateGameConnections();
}

void AppStateGameServer::HandleGameConnections()
{
	NetString string;
	Client *client = NULL;
	while (true)
	{
		std::vector<int> newClients, removedClients;
		int receiveId = networkGame->ReceiveData(&newClients, &removedClients);

		// Handle new connections
		for (std::vector<int>::iterator it = newClients.begin(); it != newClients.end(); it++)
		{
			if (clients[*it] != NULL)
			{
				availablePlayerIds[clients[*it]->player_id] = true;
				delete clients[*it];
				clientCount--;
				clients[*it] = NULL;
			}

			client = new Client();
			int pawnIndex = Ship::Add_Ship(NO_TEAM, INTERCEPTOR, 100, 100, 5.0); // Will get its teamid later
			if (pawnIndex >= 0)
				client->pawn = Ship::ships[pawnIndex];
			else
				std::cout << "[ ERROR ] " << CurrentDateTime() << " >>> Unable to create ship." << std::endl;

			// Insert logic to place new ship;

			client->offline = true;
			client->channel_id = *it;
			networkGame->Bind(*it);
			clientCount++;
			clients[*it] = client;

			SendStateUpdate(*it);

			if (debugLevel > DL_NONE)
				std::cout << "[ New Client Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;

			time(&lastActivity);
		}

		// Handle removed connections by tcp drops
		for (std::vector<int>::iterator it = removedClients.begin(); it != removedClients.end(); it++)
		{
			if (clients[*it]->team_id == RED_TEAM)
				teamRedCount--;
			else
				teamBlueCount--;

			availablePlayerIds[clients[*it]->player_id] = true;
			string.WriteUChar(NCE_REMOVE_PLAYER);
			string.WriteUChar(clients[*it]->player_id);
			delete clients[*it];
			clients[*it] = NULL;
			clientCount--;
			UpdateMainServer();

			if (debugLevel > DL_NONE)
				std::cout << "[ Client Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;

			time(&lastActivity);
		}

		// Did we get a message ready to read?
		if (receiveId == -1)
			break;

		time(&lastActivity);

		// Read that new message
		client = clients[receiveId];
		time(&client->last_input);
		NetString *netString = networkGame->GetData(receiveId);
		if (netString == NULL || netString->BufferLength() == 0)
			continue;

		bool reading = true;
		while (reading)
		{
			unsigned char temp;
			if (!netString->ReadUChar(temp))
				break;
			NetworkChunkEnums type = (NetworkChunkEnums)temp;

			switch (type)
			{
				case NCE_PLAYER_INPUT:
				{
					Parser parser;
					parser.DeserializeInput(netString, client->inputs, NUMBER_OF_INPUTS);
					break;
				}

				case NCE_LOOKUP_PLAYER:
				{
					unsigned char player_id;
					netString->ReadUChar(player_id);

					Client *player = NULL;
					for (int i = 0; i < MaximumClients; i++)
					{
						if (clients[i] != NULL && clients[i]->player_id == player_id)
						{
							player = clients[i];
							break;
						}
					}

					if (player == NULL)
						break;

					NetString response;
					response.WriteUChar(NCE_LOOKUP_PLAYER);
					response.WriteInt(player->player_id);
					response.WriteUChar(player->team_id);
					response.WriteString(player->player_name);
					response.WriteUChar(NCE_END);
					networkGame->SendData(&response, receiveId);
					break;
				}

				case NCE_PLAYER_GREETING:
				{
					netString->ReadString(client->player_name);

					bool playerExists = false;
					for (int i = 0; i < MaximumClients; i++)
					{
						if (clients[i] == NULL || clients[i] == client)
							continue;

						if (clients[i]->player_name == client->player_name)
						{
							playerExists = true;
							break;
						}
					}

					if (playerExists)
					{
						NetString response;
						response.WriteUChar(NCE_ALREADY_JOINED);
						response.WriteUChar(NCE_END);
						networkGame->SendData(&response, client->channel_id);
						clients[client->channel_id] = NULL;
						delete client;
						clientCount--;
						break;
					}

					// We got a greeting, but is it redundant?
					bool newPlayer = false;
					if (client->player_id == -1)
					{
						newPlayer = true;
						bool wasFromLobby = false;

						// Did we expect it?
						for (std::vector<Client *>::iterator it = expectedClients.begin(); it != expectedClients.end(); it++)
						{
							if ((*it)->player_name == client->player_name)
							{
								client->player_id = (*it)->player_id;
								client->team_id = (*it)->team_id;
								expectedClients.erase(it);
								wasFromLobby = true;
								break;
							}
						}

						// Unexpected, so give it a team
						if (!wasFromLobby)
						{
							if (!MakeTeamsEven(client->channel_id))
							{
								NetString response;
								response.WriteUChar(NCE_TOO_MANY_PLAYERS);
								response.WriteUChar(NCE_END);
								networkGame->SendData(&response, receiveId);
								break;
							}
							
							client->player_id = GetNextPlayerId();
						}
					}

					NetString response;
					response.WriteUChar(NCE_PLAYER_GREETING);
					response.WriteInt(client->player_id);
					response.WriteUChar(client->team_id);
					response.WriteInt(map->SEED);
					response.WriteInt(map->map_scale);
					response.WriteUChar(NCE_END);
					networkGame->SendData(&response, receiveId);

					if (newPlayer)
					{
						NetString broadcast;
						broadcast.WriteUChar(NCE_NEW_PLAYER);
						broadcast.WriteInt(client->player_id);
						broadcast.WriteUChar(client->team_id);
						broadcast.WriteString(client->player_name);
						broadcast.WriteUChar(NCE_END);
						SendToAll(&broadcast);

						UpdateMainServer();
					}
					break;
				}

				case NCE_PLAYER_EXITED:
				{
					string.WriteUChar(NCE_REMOVE_PLAYER);
					string.WriteUChar(client->player_id);
					if (client->team_id == RED_TEAM)
						teamRedCount--;
					else
						teamBlueCount--;
					availablePlayerIds[client->player_id] = true;
					clients[receiveId] = NULL;
					clientCount--;
					UpdateMainServer();

					networkGame->RemoveConnection(receiveId); // Sync with networkGame
				}

				default:
					reading = false;
			}
		}

		delete netString;
	}


	// Have any of the connections timed out?
	time_t now = time(NULL);
	for (int i = 0; i < MaximumClients; ++i)
	{
		client = clients[i];
		if (client == NULL)
			continue;

		if (now - client->last_input > 5)
		{
			string.WriteUChar(NCE_REMOVE_PLAYER);
			string.WriteUChar(clients[i]->player_id);
			if (clients[i]->team_id == RED_TEAM)
				teamRedCount--;
			else
				teamBlueCount--;
			availablePlayerIds[clients[i]->player_id] = true;
			clients[i] = NULL;
			clientCount--;
			UpdateMainServer();

			networkGame->RemoveConnection(i); // Sync with networkGame

			if (debugLevel > DL_NONE)
			{
				std::cout << "[ Client Timed Out ] " << CurrentDateTime() << " >>> Channel: " << i << std::endl;
			}

			delete client;
		}
	}

	// Have the connections we expected to join from the lobby timed out?  Remove their reserved spot on the team if so.
	for (std::vector<Client *>::reverse_iterator it = expectedClients.rbegin(); it != expectedClients.rend(); it++)
	{
		if (now - (*it)->last_input > 5)
		{
			availablePlayerIds[(*it)->player_id] = true;
			if ((*it)->team_id == RED_TEAM)
				teamRedCount--;
			else
				teamBlueCount--;

			std::cout << "Player: " << (*it)->player_name << " has not joined in time.\n";
			delete *it;
			expectedClients.erase(--(it.base()));
		}
	}

	if (string.BufferLength() > 0)
	{
		string.WriteUChar(NCE_END);
		SendToAll(&string);
	}
}

void AppStateGameServer::UpdateGameConnections()
{
	NetString string;

	Client *client;
	for (int i = 0; i < MaximumClients; ++i)
	{
		client = clients[i];
		if (client == NULL)
			continue;

		string.WriteUChar(NCE_PLAYER);
		string.WriteUChar(client->player_id);
		NetString *chunk = client->pawn->Serialize();
		string += *chunk;
		string.WriteBool(client->inputs[FIRE_ENERGY]);
		string.WriteBool(client->inputs[FIRE_BALLISTIC]);
		string.WriteBool(client->inputs[FIRE_PROPELLED]);
		string.WriteBool(client->inputs[FIRE_MINE]);
		string.WriteBool(client->inputs[FIRE_POWERUP]);
		delete chunk;
	}

	if (string.BufferLength() > 0)
	{
		string.WriteUChar(NCE_END);
		SendToAll(&string);
	}
}

void AppStateGameServer::SendToAll(NetString *string)
{
	for (int i = 0; i < MaximumClients; ++i)
	{
		if (clients[i] == NULL)
			continue;

		networkGame->SendData(string, i);
	}

	if (debugLevel > DL_MED)
	{
		std::cout << "[ Message Sent To All Clients ] " << CurrentDateTime() << " >>> " << *string << std::endl;
	}
}

void AppStateGameServer::Draw() {
}

void AppStateGameServer::Cleanup() {
	networkGame->Close();
	delete networkGame;
}

AppStateBase * AppStateGameServer::GetInstance() {
	if (instance != NULL)
		delete instance;
		
	instance = new AppStateGameServer();
	return instance;
}

void AppStateGameServer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
}

void AppStateGameServer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
}

const std::string CurrentDateTime()
{
	time_t now = time(NULL);
	struct tm tstruct;
	char buffer[80];
	tstruct = *localtime(&now);
	strftime(buffer, 80, "%m/%d/%Y %X", &tstruct);
	return std::string(buffer);
}