#include "HeadServer.h"

HeadServer::HeadServer()
{
	networkLobbies = NetworkFactory::getInstance("./conf/networkLobbies.conf");
	networkLobbies->Init(true);
	networkPlayers = NetworkFactory::getInstance("./conf/networkPlayers.conf");
	networkPlayers->Init(true);

	for (int i = 0; i < MaximumClients; ++i)
		activePlayers[i] = false;

	for (int i = 0; i < AvailablePortsSize; ++i)
		availablePorts[i] = true;
}

HeadServer::~HeadServer()
{
	networkLobbies->Close();
	networkPlayers->Close();
	for (int i = 0; i < lobbies.size(); i++)
	{
		if (lobbies[i] == NULL)
			continue;
		delete lobbies[i];
	}
}

bool HeadServer::Update()
{
	////////////////////////////////////////////////////////////////
	// Clean up lobbies
	if (lobbies.size() > 0 && lobbies.front() == NULL)
		lobbies.pop_front();
	if (lobbies.size() > 0 && lobbies.back() == NULL)
		lobbies.pop_back();
	////////////////////////////////////////////////////////////////

	ReceiveLobbies();
	ReceivePlayers();

	return false;
}

void HeadServer::ReceivePlayers()
{
	while (true)
	{
		std::vector<int> newPlayers, removedPlayers;
		int receiveId = networkPlayers->ReceiveData(&newPlayers, &removedPlayers);

		// Handle new connections
		for (std::vector<int>::iterator it = newPlayers.begin(); it != newPlayers.end(); it++)
		{
			activePlayers[*it] = true;

			NotifyPlayers(*it);
			std::cout << "[ New Player Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}

		for (std::vector<int>::iterator it = removedPlayers.begin(); it != removedPlayers.end(); it++)
		{
			activePlayers[*it] = false;

			std::cout << "[ Player Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}

		// Did we get a message ready to read?
		if (receiveId == -1)
			break;

		NetString *netString = networkPlayers->GetData(receiveId);
		bool reading = true;
		while (reading)
		{
			unsigned char temp;
			if (!netString->ReadUChar(temp))
				break;
			NetworkChunkEnums type = (NetworkChunkEnums)temp;

			switch (type)
			{
				case NCE_PLAYER_LOGIN:
				{
					std::string name, password;
					netString->ReadString(name);
					netString->ReadString(password);

					bool pass = PerformLogin(name, password);

					NetString response;
					response.WriteUChar(NCE_PLAYER_LOGIN);
					response.WriteBool(pass);
					response.WriteUChar(NCE_END);
					networkPlayers->SendData(&response, receiveId);
					break;
				}

				case NCE_CREATE_LOBBY:
				{
					std::string name;
					netString->ReadString(name);
					float scale;
					netString->ReadFloat(scale);

					int port = 0;
					for (int i = 0; i < AvailablePortsSize; ++i)
					{
						if (availablePorts[i])
						{
							port = PortRangeStart + i;
							break;
						}
					}

					if (port != 0)
					{
						if (CreateLobby(name, port, scale))
						{
							NetString response;
							response.WriteUChar(NCE_CREATE_LOBBY);
							response.WriteString(name);
							response.WriteInt(port);
							response.WriteUChar(NCE_END);
							networkPlayers->SendData(&response, receiveId);
						}
					}
					else
					{
						// Handle errors
					}

					break;
				}

				case NCE_END:
					break;

				default:
					reading = false;
					break;
			}
		}
	}
}

void HeadServer::ReceiveLobbies()
{
	bool updatePlayers = false;

	while (true)
	{
		std::vector<int> newLobbies, removedLobbies;
		int receiveId = networkLobbies->ReceiveData(&newLobbies, &removedLobbies);

		// Handle new connections
		for (std::vector<int>::iterator it = newLobbies.begin(); it != newLobbies.end(); it++)
		{
			updatePlayers = true;
			
			Lobby *lobby = new Lobby();
			IPaddress *address = networkLobbies->GetIPAddress(*it);
			if (address == NULL)
			{
				lobby->address.host = 0;
				lobby->address.port = 0;
			}
			else
			{
				lobby->address.host = address->host;
				lobby->address.port = address->port;
			}

			lobby->channelId = *it;
			lobbies.push_back(lobby);
			AddLobby(*lobby);

			std::cout << "[ New Lobby Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}

		for (std::vector<int>::iterator it = removedLobbies.begin(); it != removedLobbies.end(); it++)
		{
			updatePlayers = true;

			Lobby *lobby = NULL;
			for (int i = 0; i < lobbies.size(); i++)
			{
				if (lobbies[i] != NULL && lobbies[i]->channelId == *it)
				{
					lobby = lobbies[i];
					lobbies[i] = NULL;
					break;
				}
			}

			if (lobby != NULL)
			{
				// Was the lobby on localhost?
				if (lobby->address.host == LocalhostInt)
				{
					int portOffset = lobby->gamePort - PortRangeStart;
					availablePorts[portOffset] = true;
				}

				RemoveLobby(*lobby);
				delete lobby;

				std::cout << "[ Lobby Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
			}
		}

		// Did we get a message ready to read?
		if (receiveId == -1)
			break;

		Lobby *lobby = NULL;
		for (int i = 0; i < lobbies.size(); i++)
		{
			if (lobbies[i] != NULL && lobbies[i]->channelId == receiveId)
			{
				lobby = lobbies[i];
				break;
			}
		}

		if (lobby == NULL)
		{
			NetString *netString = networkLobbies->GetData(receiveId);
			std::cout << "[ ERROR ] " << CurrentDateTime() << " >>> Message received from untracked lobby: " << receiveId << "\n";
			if (netString != NULL)
			{
				std::cout << *netString << '\n';
				delete netString;
			}

			networkLobbies->RemoveConnection(receiveId);
			updatePlayers = true;
			break;
		}

		NetString *netString = networkLobbies->GetData(receiveId);
		if (netString == NULL)
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
				case NCE_LOBBY:
				{
					netString->ReadString(lobby->name);

					int temp;
					netString->ReadInt(temp);
					lobby->gamePort = (Uint32)temp;

					// Was the lobby on localhost?
					if (lobby->address.host == LocalhostInt)
					{
						int portOffset = lobby->gamePort - PortRangeStart;
						availablePorts[portOffset] = false;
					}

					unsigned char tempc;
					netString->ReadUChar(tempc);
					lobby->state = (GameServerEnums)tempc;

					netString->ReadUChar(lobby->playerCount);
					netString->ReadInt(lobby->mapSeed);
					netString->ReadFloat(lobby->mapScale);

					// std::cout << "Got: " << lobby->name << "; port: " << lobby->gamePort << "; state: " << lobby->state << "; players: " << (int)lobby->playerCount << '\n';
					SaveLobby(*lobby);
					updatePlayers = true;
					break;
				}

				case NCE_END:
					break;

				default:
					reading = false;
					break;
			}
		}

		delete netString;
	}

	if (updatePlayers)
		NotifyPlayers();
}

void HeadServer::NotifyPlayers(char id)
{
	NetString string;
	string.WriteUChar(NCE_LOBBY_SYNC);

	for (int i = 0; i < lobbies.size(); i++)
	{
		if (lobbies[i] == NULL)
			continue;

		string.WriteUChar(NCE_LOBBY);
		NetString *temp = lobbies[i]->Serialize();
		if (temp != NULL)
			string += *temp;
		delete temp;
	}

	string.WriteUChar(NCE_END);

	if (id == -1)
	{
		for (int i = 0; i < MaximumClients; i++)
			networkPlayers->SendData(&string, i);
	}
	else
		networkPlayers->SendData(&string, id);
}

bool HeadServer::PerformLogin(std::string name, std::string password)
{
	// just let anyone using the name test through
	if (name.substr(0, 4) == "test" || name.substr(0, 4) == "Test")
		return true;

	bool pass = false;

	Database database;
	if (database.connect())
	{
		int playerId = database.check_player((char *)name.c_str(), (char *)password.c_str());
		pass = playerId > 0;
	}
	else
		std::cerr << "[ ERROR ] " << CurrentDateTime() << " >>> Unable to perform login." << std::endl;

	database.close();

	return pass;
}

bool HeadServer::AddLobby(Lobby &lobby)
{
	bool success = false;

	Database database;
	if (database.connect())
		success = database.insert_lobby((int)lobby.channelId);
	else
		std::cerr << "[ ERROR ] " << CurrentDateTime() << " >>> Unable to add lobby." << std::endl;

	database.close();
	return success;
}

bool HeadServer::SaveLobby(Lobby &lobby)
{
	bool success = false;

	Database database;
	if (database.connect())
		success = database.update_lobby_num_players((int)lobby.playerCount, (int)lobby.channelId);
	else
		std::cerr << "[ ERROR ] " << CurrentDateTime() << " >>> Unable to update lobby." << std::endl;

	database.close();
	return success;
}

bool HeadServer::RemoveLobby(Lobby &lobby)
{
	bool success = false;

	Database database;
	if (database.connect())
		success = database.delete_lobby((int)lobby.channelId);
	else
		std::cerr << "[ ERROR ] " << CurrentDateTime() << " >>> Unable to delete lobby." << std::endl;

	database.close();
	return success;
}

bool HeadServer::CreateLobby(std::string name, int port, float mapScale)
{
	const char * logLocation = "./lobbiesLog";
	char portBuffer[10];
	char scaleBuffer[30];

	std::stringstream stream;
	stream << port;
	memcpy(portBuffer, stream.str().c_str(), stream.str().length() + 1);

	stream.str(std::string());
	stream << mapScale;
	memcpy(scaleBuffer, stream.str().c_str(), stream.str().length() + 1);

	stream.str(std::string());
	stream << logLocation << port << ".log";

	char* const args[] = { (char*)"./starclash", (char*)"server", portBuffer, (char*)name.c_str(), scaleBuffer, (char *)0 };

	int pid = fork();
	if (pid == -1)
		return false;

	// Am i the child?
	if (pid == 0)
	{
		// Redirect stdout and stderr
		int fd = open(stream.str().c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
		dup2(fd, 1);
		dup2(fd, 2);

		chdir("../app");
		execv("./starclash", args);

		// This is ran when an error occurs
		exit(1);
	}

	return true;
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