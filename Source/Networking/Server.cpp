#include "Server.h"

Server::Server(DebugLevel l)
	: debugLevel(l), checkForInactivity(1)
{
	network = NetworkFactory::getInstance();
	network->Init();

	for (int i = 0; i < MaximumClients; ++i)
	{
		clientList[i] = NULL;
	}

	std::cout << "\n[ Server Started ] " << CurrentDateTime() << "\n";
}

Server::~Server()
{
	network->Close();
}

std::vector<int> Server::HandleIncomingData()
{
	std::vector<int> newServerClients;

	while (network->ReceiveData())
	{
		bool exists = false;
		ServerClient *client = NULL;

		for (int i = 0; i < clients.size(); i++)
		{
			if (clients[i] == NULL)
				continue;

			if (clients[i]->ip == network->GetRecvHost() && clients[i]->port == network->GetRecvPort())
			{
				exists = true;
				client = clients[i];
				break;
			}
		}

		if (!exists)
		{
			int nextChannel = GetNextChannel();
			if (nextChannel != -1)
			{
				usedChannels[nextChannel] = true;
				client = new ServerClient(network->GetRecvHost(), network->GetRecvPort(), nextChannel);
				clients.push_back(client);
				clientList[nextChannel] = client;
				time(&client->lastInput);
				newServerClients.push_back(nextChannel);

				NetString netString;
				netString.WriteUChar(NCE_NEW_PLAYER);
				netString.WriteUChar(nextChannel);
				netString.WriteUChar(NCE_END);

				network->SendData(&netString, -1);
				network->Bind(nextChannel);

				if (debugLevel > DL_NONE)
				{
					std::cout << "\n[ New Client Accepted ] " << CurrentDateTime() << "\nChannel: " << nextChannel << "\n";
				}
			}
			else
			{
				NetString netString;
				netString.WriteUChar(NCE_TOO_MANY_PLAYERS);
				netString.WriteUChar(NCE_END);

				network->SendData(&netString, -1);
				if (debugLevel > DL_NONE)
				{
					std::cout << "\n[ New Client Rejected ] " << CurrentDateTime() << "\nToo many connections.\n";
				}
			}
		}
		else if (client != NULL)
		{
			time(&client->lastInput);
			NetString *netString = network->GetData();
			Parser parser;
			parser.DeserializeInput(netString, client->inputs);
			delete netString;
		}
	}

	return newServerClients;
}

std::bitset<MaximumClients> Server::CleanupClients()
{
	std::bitset<MaximumClients> cleanedUpClients;
	cleanedUpClients.reset();

	if (clients.size() > 0 && clients.front() == NULL)
		clients.pop_front();
	if (clients.size() > 0 && clients.back() == NULL)
		clients.pop_back();

	// Check for inactive clients
	if (checkForInactivity-- == 0)
	{
		checkForInactivity = 200;

		time_t now = time(NULL);
		ServerClient *client = NULL;
		NetString string;

		for (int i = 0; i < clients.size(); ++i)
		{
			client = clients[i];
			if (client == NULL)
				continue;

			if (now - client->lastInput > 5)
			{
				cleanedUpClients[client->channel] = true;
				clientList[client->channel] = NULL;
				clients[i] = NULL;
				usedChannels[client->channel] = false;

				string.WriteUChar(NCE_REMOVE_PLAYER);
				string.WriteUChar((unsigned char)client->channel);

				if (debugLevel > DL_NONE)
				{
					std::cout << "\n[ Client Timed Out ] " << CurrentDateTime() << "\nChannel: " << client->channel << "\n";
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

	return cleanedUpClients;
}

void Server::SendToAll(NetString *string)
{
	ServerClient *client = NULL;
	for (int i = 0; i < clients.size(); ++i)
	{
		client = clients[i];
		if (client == NULL)
			continue;

		network->SendData(string, client->channel);
	}

	if (debugLevel > DL_MED)
	{
		std::cout << "\n[ Message Sent To All Clients ] " << CurrentDateTime() << "\n" << *string << "\n";
	}
}

int Server::GetNextChannel()
{
	for (int i = 0; i < MaximumClients; i++)
	{
		if (!usedChannels.test(i))
			return i;
	}

	return -1;
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