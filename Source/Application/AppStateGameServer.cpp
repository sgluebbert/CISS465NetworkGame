#include "AppStateGameServer.h"


AppStateBase * AppStateGameServer::instance = NULL;



AppStateGameServer::AppStateGameServer(DebugLevel l)
	: debugLevel(l), clientCount(0), secondsToStart(10), map(100)
{
	time(&secondsToStartLastTick);
	for (int i = 0; i < MaximumClients; i++)
		clients[i] = NULL;
}

void AppStateGameServer::Initialize() {
	network = NetworkFactory::getInstance();
	network->Init(true);
	time(&lastSpeedDisplay);
	std::cout << "[ Server Started ] " << CurrentDateTime() << std::endl;
}

void AppStateGameServer::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGameServer::Update() {
    if (debugLevel > DL_LOW)
	{
		time_t now;
		time(&now);
		if (now - lastSpeedDisplay >= 1)
		{
			time(&lastSpeedDisplay);
			if (clientCount > 0)
				std::cout << "[ Server Speed ] " << CurrentDateTime() << " >>> Clients: " << clientCount << "; Frames Per Second: " << Clock::Frame_Control.Get_FPS() << std::endl;
		}
	}

	// Handle new/removed connections and their input
	HandleConnections();

	// Fix this
	if (clientCount < map.MIN_NUMBER_OF_PLAYERS_PER_TEAM)
		return;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Game Logic
	double dt = Clock::Frame_Control.Get_Time_Per_Frame();

	if (secondsToStart == 0)
	{
		for (int i = 0; i < MaximumClients; i++)
		{
			if (clients[i] == NULL)
				continue;
		    clients[i]->Update(dt);
		}
	}
	else
	{
		time_t now;
		time(&now);
		if (now - secondsToStartLastTick >= 1)
		{
			time(&secondsToStartLastTick);
			secondsToStart--;
			SendSecondsToStart();
		}
	}

	for (int i = 0; i < Rigid_Body::objects.size(); i++)
		if (Rigid_Body::objects[i] != NULL)
			Rigid_Body::objects[i]->Update(dt);
    /////////////////////////////////////////////////////////////////////////////////////////

	// Update all of the connections to the game state
	UpdateConnections();
}

void AppStateGameServer::HandleConnections()
{
	NetString string;
	Client *client = NULL;
	while (true)
	{
		std::vector<int> newClients, removedClients;
		int receiveId = network->ReceiveData(&newClients, &removedClients);

		// Handle new connections
		for (std::vector<int>::iterator it = newClients.begin(); it != newClients.end(); it++)
		{
			if (clients[*it] != NULL)
			{
				delete clients[*it];
				clientCount--;
				clients[*it] = NULL;
			}

			client = new Client();
			client->offline = true;
			client->channel_id = *it;
			network->Bind(*it);
			clientCount++;
			clients[*it] = client;

			SendSecondsToStart(*it);

			if (debugLevel > DL_NONE)
				std::cout << "[ New Client Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}

		// Handle removed connections by tcp drops
		for (std::vector<int>::iterator it = removedClients.begin(); it != removedClients.end(); it++)
		{
			delete clients[*it];
			clients[*it] = NULL;
			clientCount--;
			string.WriteUChar(NCE_REMOVE_PLAYER);
			string.WriteUChar((unsigned char)*it);

			if (debugLevel > DL_NONE)
			{
				std::cout << "[ Client Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
			}
		}

		// Did we get a message ready to read?
		if (receiveId == -1)
			break;


		// Read that new message
		client = clients[receiveId];
		time(&client->last_input);
		NetString *netString = network->GetData(receiveId);
		Parser parser;
		parser.DeserializeInput(netString, client->inputs, NUMBER_OF_INPUTS);
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
			string.WriteUChar((unsigned char)i);
			clients[i] = NULL;
			clientCount--;

			network->RemoveConnection(i); // Sync with network

			if (debugLevel > DL_NONE)
			{
				std::cout << "[ Client Timed Out ] " << CurrentDateTime() << " >>> Channel: " << i << std::endl;
			}

			delete client;
		}
	}

	if (string.BufferLength() > 0)
	{
		string.WriteUChar(NCE_END);
		SendToAll(&string);
	}
}

void AppStateGameServer::UpdateConnections()
{
	NetString string;

	Client *client;
	for (int i = 0; i < MaximumClients; ++i)
	{
		client = clients[i];
		if (client == NULL)
			continue;

		string.WriteUChar(NCE_PLAYER);
		string.WriteUChar(i);
		NetString *chunk = client->pawn->Serialize();
		string += *chunk;
		string.WriteBool(client->fired[ENERGY_TYPE]);
		string.WriteBool(client->fired[BALLISTIC_TYPE]);
		string.WriteBool(client->fired[PROPELLED_TYPE]);
		string.WriteBool(client->fired[BOMB_TYPE]);
		string.WriteBool(client->fired[POWERUP_TYPE]);
		delete chunk;
	}

	if (string.BufferLength() > 0)
	{
		string.WriteUChar(NCE_END);
		SendToAll(&string);
	}
}

void AppStateGameServer::SendSecondsToStart(int id)
{
	NetString string;
	string.WriteUChar(NCE_START_GAME_TIMER);
	string.WriteUChar(secondsToStart);
	string.WriteInt(secondsToStartLastTick);
	string.WriteUChar(NCE_END);

	if (id == -1)
		SendToAll(&string);
	else
		network->SendData(&string, id);
}

void AppStateGameServer::SendToAll(NetString *string)
{
	for (int i = 0; i < MaximumClients; ++i)
	{
		if (clients[i] == NULL)
			continue;

		network->SendData(string, i);
	}

	if (debugLevel > DL_MED)
	{
		std::cout << "[ Message Sent To All Clients ] " << CurrentDateTime() << " >>> " << *string << std::endl;
	}
}

void AppStateGameServer::Draw() {
    
}

void AppStateGameServer::Cleanup() {
	network->Close();
}

AppStateBase * AppStateGameServer::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGameServer();
    return instance;
}

void AppStateGameServer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}

void AppStateGameServer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
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