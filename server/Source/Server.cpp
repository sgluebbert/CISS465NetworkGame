#include "Server.h"

Server::Server(DebugLevel l)
	: debugLevel(l), checkForInactivity(1), frameCount(0), clientCount(0)
{
	time(&lastSpeedDisplay);
	network = NetworkFactory::getInstance();
	network->Init(true);

	for (int i = 0; i < MaximumClients; ++i)
	{
		clientList[i] = NULL;
	}

	std::cout << "[ Server Started ] " << CurrentDateTime() << std::endl;
}

Server::~Server()
{
	network->Close();
}

std::vector<int> Server::HandleIncomingData()
{
	std::vector<int> newServerClients;
	std::vector<int> newPlayers;
	removedClients.clear();

	while (true)
	{
		ServerClient *client = NULL;

		newServerClients.clear();
		int receiveId = network->ReceiveData(&newServerClients, &removedClients);
		if (newServerClients.size() > 0)
		{
			for (std::vector<int>::iterator it = newServerClients.begin(); it != newServerClients.end(); it++)
			{
				if (clientList[*it] != NULL)
				{
					delete clientList[*it];
					clientCount--;
					clientList[*it] = NULL;
				}

				client = new ServerClient(*it);
				network->Bind(*it);
				clientCount++;
				clientList[*it] = client;
				time(&client->lastInput);

				if (debugLevel > DL_NONE)
				{
					std::cout << "[ New Client Accepted ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
				}

				newPlayers.push_back(*it);
			}
		}

		if (receiveId == -1)
			break;

		client = clientList[receiveId];
		time(&client->lastInput);
		NetString *netString = network->GetData(receiveId);
		Parser parser;
		parser.DeserializeInput(netString, client->inputs, NUMBER_OF_INPUTS);
	}

	if (debugLevel > DL_LOW)
		CheckSpeed();

	return newPlayers;
}

std::bitset<MaximumClients> Server::CleanupClients()
{
	std::bitset<MaximumClients> cleanedUpClients;
	cleanedUpClients.reset();
	NetString string;

	// Removed from network earlier
	for (std::vector<int>::iterator it = removedClients.begin(); it != removedClients.end(); it++)
	{
		cleanedUpClients[*it] = true;
		delete clientList[*it];
		clientList[*it] = NULL;
		clientCount--;
		string.WriteUChar(NCE_REMOVE_PLAYER);
		string.WriteUChar((unsigned char)*it);

		if (debugLevel > DL_NONE)
		{
			std::cout << "[ Client Connection Dropped ] " << CurrentDateTime() << " >>> Channel: " << *it << std::endl;
		}
	}

	// Check for inactive clients
	if (checkForInactivity-- == 0)
	{
		checkForInactivity = 200;

		time_t now = time(NULL);
		ServerClient *client = NULL;

		for (int i = 0; i < MaximumClients; ++i)
		{
			client = clientList[i];
			if (client == NULL)
				continue;

			if (now - client->lastInput > 5)
			{
				string.WriteUChar(NCE_REMOVE_PLAYER);
				string.WriteUChar((unsigned char)i);
				clientList[i] = NULL;
				clientCount--;

				network->RemoveConnection(i); // Only used in UDP connections

				if (debugLevel > DL_NONE)
				{
					std::cout << "[ Client Timed Out ] " << CurrentDateTime() << " >>> Channel: " << i << std::endl;
				}

				delete client;
			}
		}

	}

	if (string.BufferLength() > 0)
	{
		string.WriteUChar(NCE_END);
		SendToAll(&string);
	}

	return cleanedUpClients;
}

void Server::SendToAll(NetString *string)
{
	ServerClient *client = NULL;
	for (int i = 0; i < MaximumClients; ++i)
	{
		client = clientList[i];
		if (client == NULL)
			continue;

		network->SendData(string, i);
	}

	if (debugLevel > DL_MED)
	{
		std::cout << "[ Message Sent To All Clients ] " << CurrentDateTime() << " >>> " << *string << std::endl;
	}
}

void Server::CheckSpeed()
{
	if (clientCount == 0)
	{
		time(&lastSpeedDisplay);
		return;
	}

	frameCount++;

	const float timespan = 2;
	time_t now;
	time(&now);
	if (now - lastSpeedDisplay >= timespan)
	{
		time(&lastSpeedDisplay);
		std::cout << "[ Server Speed ] " << CurrentDateTime() << " >>> Clients: " << clientCount << "; Frames Per Second: " << frameCount / timespan << std::endl;
		frameCount = 0;
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