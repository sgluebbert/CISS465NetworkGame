#include "HeadServer.h"

HeadServer::HeadServer()
{
	networkLobbies = NetworkFactory::getInstance("./conf/networkLobbies.conf");
	networkLobbies->Init(true);
	networkPlayers = NetworkFactory::getInstance("./conf/networkPlayers.conf");
	networkPlayers->Init(true);

	for (int i = 0; i < MaximumClients; ++i)
		activePlayers[i] = false;
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
			updatePlayers = true;

			std::cout << "[ New Lobby Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}

		for (std::vector<int>::iterator it = removedLobbies.begin(); it != removedLobbies.end(); it++)
		{
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
				delete lobby;

				std::cout << "[ Lobby Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
			}

			updatePlayers = true;
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

					unsigned char tempc;
					netString->ReadUChar(tempc);
					lobby->state = (GameServerEnums)tempc;

					netString->ReadUChar(lobby->playerCount);

					std::cout << "Got: " << lobby->name << "; port: " << lobby->gamePort << "; state: " << lobby->state << "; players: " << (int)lobby->playerCount << '\n';
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

const std::string CurrentDateTime()
{
	time_t now = time(NULL);
	struct tm tstruct;
	char buffer[80];
	tstruct = *localtime(&now);
	strftime(buffer, 80, "%m/%d/%Y %X", &tstruct);
	return std::string(buffer);
}