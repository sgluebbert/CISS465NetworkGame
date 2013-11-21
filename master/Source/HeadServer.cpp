#include "HeadServer.h"

HeadServer::HeadServer()
{
	network = NetworkFactory::getInstance();
	network->Init(true);
}

HeadServer::~HeadServer()
{
	network->Close();
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

	Receive();

	return false;
}

void HeadServer::Receive()
{
	while (true)
	{
		std::vector<int> newLobbies, removedLobbies;
		int receiveId = network->ReceiveData(&newLobbies, &removedLobbies);

		// Handle new connections
		for (std::vector<int>::iterator it = newLobbies.begin(); it != newLobbies.end(); it++)
		{
			Lobby *lobby = new Lobby();
			IPaddress *address = network->GetIPAddress(*it);
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
			std::cout << "[ ERROR ] " << CurrentDateTime() << " >>> Message received from untracked lobby: " << receiveId << "\n";
			break;
		}

		NetString *netString = network->GetData(receiveId);
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

const std::string CurrentDateTime()
{
	time_t now = time(NULL);
	struct tm tstruct;
	char buffer[80];
	tstruct = *localtime(&now);
	strftime(buffer, 80, "%m/%d/%Y %X", &tstruct);
	return std::string(buffer);
}