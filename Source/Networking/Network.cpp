#include "Network.h"

Network* NetworkFactory::instance = NULL;
NetworkType NetworkFactory::networkType = UNDEFINED;

////////////////////////////////////////////////////////////////////////////////////////////
// UDP
int UDPNetwork::Init(bool isServer)
{
	_setSize = (isServer ? MaximumClients : 1);
	
	NetworkParser * networkParser = new NetworkParser();

		Uint16 listen_port;
		if (isServer)
			listen_port = networkParser->GetServerPort();
		else
			listen_port = networkParser->GetClientPort();
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	if (!(_sd = SDLNet_UDP_Open(listen_port)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	if (!(_receive = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	if (!(_send = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

		IPaddress host_address;
	const char * host_ipaddress = networkParser->GetHostAddress();
	if (isServer)
		host_ipaddress = NULL;
	Uint32 host_port;
	if (isServer)
		host_port = networkParser->GetClientPort();
	else
		host_port = networkParser->GetServerPort();

	if (SDLNet_ResolveHost(&host_address, host_ipaddress, host_port) == -1) 
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", host_port, SDLNet_GetError());
		return -1;
	}

	_lastPendingIndex = 0;
	for (int i = 0; i < _setSize; ++i)
	{
		_usedSockets[i] = NULL;
		_pendingData[i] = false;
	}

	_receive->address.host = host_address.host;
	_receive->address.port = host_address.port;

	if (isServer)
		std::cout << "Connecting Via: " << get_type() << "; Listening on port " << networkParser->GetServerPort() << std::endl;
	else
		std::cout << "Connecting Via: " << get_type() << " to " << networkParser->GetHostAddress() << ":" << networkParser->GetServerPort() << std::endl;

	return 1;
}

void UDPNetwork::Close()
{
	SDLNet_FreePacket(_receive);
	SDLNet_FreePacket(_send);
	SDLNet_Quit();
}

bool UDPNetwork::SendData(NetString* stream, int id)
{
	if (id == -1)
	{
		_send->address.host = _receive->address.host;        /* Set the destination host */
		_send->address.port = _receive->address.port;        /* And destination port */
	}

	NetString *framed = stream->ToNetworkBuffer();
	int dataToSend = framed->BufferLength();

	int dataSent = 0;
	while (dataSent < dataToSend)
	{
		memcpy(_send->data, framed->Buffer() + dataSent, std::min(512, dataToSend - dataSent));
		_send->len = std::min(512, dataToSend - dataSent);
		SDLNet_UDP_Send(_sd, id, _send);
		dataSent += std::min(512, dataToSend - dataSent);
	}

	delete framed;

	return true;
}

int UDPNetwork::ReceiveData(std::vector<int> *newClients, std::vector<int> *removedClients)
{
	int result = SDLNet_UDP_Recv(_sd, _receive);
	if (result <= 0)
		return -1;

	int freeSpace = -1;
	int id = -1;
	for (int i = 0; i < _setSize; ++i)
	{
		if (_usedSockets[i] == NULL)
		{
			if (freeSpace == -1)
				freeSpace = i;
			continue;
		}

		if (_usedSockets[i]->ip == _receive->address.host && _usedSockets[i]->port == _receive->address.port)
		{
			id = i;
			break;
		}
	}

	if (id == -1)
	{
		if (freeSpace != -1)
		{
			_usedSockets[freeSpace] = new Connection(_receive->address.host, _receive->address.port);
			id = freeSpace;
			if (newClients != NULL)
				newClients->push_back(id);

			NetString netString;
			netString.WriteUChar(NCE_NEW_PLAYER);
			netString.WriteUChar(id);
			netString.WriteUChar(NCE_END);
			SendData(&netString, -1);
		}
		else
		{
			// Too many connections
			NetString netString;
			netString.WriteUChar(NCE_TOO_MANY_PLAYERS);
			netString.WriteUChar(NCE_END);
			SendData(&netString, -1);
		}
	}

	_pendingData[id] = _netStrings[id].AddNetworkBuffer(_receive->data, _receive->len);
	if (_pendingData[id])
		_netStrings[id].Seek(0);

	// Loop through the pending data and pick one, but start from the next position each time.
	int i = _lastPendingIndex;
	if (i >= _setSize)
		i = 0;
	do
	{
		if (_pendingData[i])
		{
			_pendingData[i] = false;
			_lastPendingIndex++;
			if (_lastPendingIndex >= _setSize)
				_lastPendingIndex = 0;
			return i;
		}

		i++;
		if (i >= _setSize)
			i = 0;
	}
	while (i != _lastPendingIndex);

	_lastPendingIndex++;
	if (_lastPendingIndex >= _setSize)
		_lastPendingIndex = 0;

	return -1;
}

NetString *UDPNetwork::GetData(int id)
{
	return &_netStrings[id];
}

int UDPNetwork::GetDataLength(int id)
{
	return _netStrings[id].BufferLength();
}

void UDPNetwork::RemoveConnection(int id)
{
	delete _usedSockets[id];
	_usedSockets[id] = NULL;
}

void UDPNetwork::Bind(int id)
{
	SDLNet_UDP_Bind(_sd, id, &_receive->address);
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// TCP
int TCPNetwork::Init(bool isServer)
{
	_isServer = isServer;
	_setSize = (isServer ? MaximumClients : 1);
	NetworkParser * networkParser = new NetworkParser();

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	const char * host_ipaddress = networkParser->GetHostAddress();
	if (isServer)
		host_ipaddress = NULL;
	IPaddress host_address;
	Uint16 listen_port;
	listen_port = networkParser->GetServerPort();

	// std::cout << host_ipaddress << ' ' << listen_port << '\n';
	if (SDLNet_ResolveHost(&host_address, host_ipaddress, listen_port) == -1) 
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", listen_port, SDLNet_GetError());
		return -1;
	}
	
	if (!(_sd = SDLNet_TCP_Open(&host_address)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	// If we are the server, we want to listen to be able to listen to multiple connections
	_set = SDLNet_AllocSocketSet(_setSize + 1);
	if (_set == NULL)
	{
		fprintf(stderr, "SDLNet_AllocSocketSet(%i): %s\n", _setSize, SDLNet_GetError());
		return -1;
	}

	_listnerCount = 0;
	_lastPendingIndex = 0;
	for (int i = 0; i < _setSize; ++i)
	{
		_listenSockets[i] = NULL;
		_freeSockets[i] = true;
		_pendingData[i] = false;
	}

	// Listen to our socket
	SDLNet_TCP_AddSocket(_set, _sd);

	if (isServer)
		std::cout << "Connecting Via: " << get_type() << "; Listening on port " << networkParser->GetServerPort() << std::endl;
	else
		std::cout << "Connecting Via: " << get_type() << " to " << networkParser->GetHostAddress() << ":" << networkParser->GetServerPort() << std::endl;

	return 1;
}

void TCPNetwork::Close()
{
	SDLNet_TCP_Close(_sd);
	SDLNet_Quit();
}

bool TCPNetwork::SendData(NetString* stream, int id)
{
	NetString *framed = stream->ToNetworkBuffer();
	int dataToSend = framed->BufferLength();

	int dataSent = 0;
	while (dataSent < dataToSend)
	{
		if (_isServer)
		{
			if (_listenSockets[id] == NULL)
			{
				delete framed;
				return false;
			}

			dataSent += SDLNet_TCP_Send(_listenSockets[id], framed->Buffer() + dataSent, std::min(512, dataToSend - dataSent));
		}
		else
		{
			dataSent += SDLNet_TCP_Send(_sd, framed->Buffer() + dataSent, std::min(512, dataToSend - dataSent));
		}
	}

	delete framed;
	return true;
}

int TCPNetwork::ReceiveData(std::vector<int> *newClients, std::vector<int> *removedClients)
{
	int readyCount = SDLNet_CheckSockets(_set, 0); // listen for connections up to 500 ms
	// if (readyCount == 0)
	// 	return 0;

	// Is there a new client?
	int newSocketReady = SDLNet_SocketReady(_sd);
	if (newSocketReady != 0)
	{
		if (_isServer)
		{
			if (_listnerCount < _setSize)
			{
				int freeSpot = 0;
				for (int i = 0; i < _setSize; i++)
				{
					if (_freeSockets[i])
					{
						freeSpot = i;
						_freeSockets[i] = false;
						break;
					}
				}

				// Accept new socket
				_listenSockets[freeSpot] = SDLNet_TCP_Accept(_sd);
				_freeSockets[freeSpot] = false;
				SDLNet_TCP_AddSocket(_set, _listenSockets[freeSpot]);
				if (newClients != NULL)
					newClients->push_back(freeSpot);
				NetString netString;
				netString.WriteUChar(NCE_NEW_PLAYER);
				netString.WriteUChar(freeSpot);
				netString.WriteUChar(NCE_END);
				SendData(&netString, freeSpot);
				_listnerCount++;
			}
			else
			{
				// Reject new socket
				TCPsocket temp = SDLNet_TCP_Accept(_sd);
				NetString netString;
				netString.WriteUChar(NCE_TOO_MANY_PLAYERS);
				netString.WriteUChar(NCE_END);
				
				NetString *framed = netString.ToNetworkBuffer();
				SDLNet_TCP_Send(temp, framed->Buffer(), framed->BufferLength());
				delete framed;
				SDLNet_TCP_Close(temp);
			}
		}
	}

	unsigned char buffer[512];
	if (_isServer)
	{
 		for (int i = 0; i < _setSize; ++i)
 		{
 			if (_listenSockets == NULL)
 				continue;

 			int socketReady = SDLNet_SocketReady(_listenSockets[i]);

 			// Socket has stuff to say
 			if (socketReady != 0)
 			{
 				int receivedCount = SDLNet_TCP_Recv(_listenSockets[i], buffer, 512);

 				// Did connection disconnect?
 				if (receivedCount <= 0)
 				{
 					if (removedClients != NULL)
 						removedClients->push_back(i);
 					SDLNet_TCP_DelSocket(_set, _listenSockets[i]);
 					SDLNet_TCP_Close(_listenSockets[i]);
 					_listenSockets[i] = NULL;
 					_freeSockets[i] = true;
 					_listnerCount--;
 				}
 				else
 				{
 					_pendingData[i] = _netStrings[i].AddNetworkBuffer(buffer, receivedCount);
					if (_pendingData[i])
						_netStrings[i].Seek(0);
 				}
 			}
 		}
 	}
 	else
 	{
 		int socketReady = SDLNet_SocketReady(_sd);
 		if (socketReady != 0)
		{
			int receivedCount = SDLNet_TCP_Recv(_sd, buffer, 512);

			if (receivedCount <= 0)
			{
				// Handle server closing connection
			}
			else
			{
				_pendingData[0] = _netStrings[0].AddNetworkBuffer(buffer, receivedCount);
				if (_pendingData[0])
				{
					_netStrings[0].Seek(0);
					return 0;
				}
			}
		}

		return -1;
 	}

	int i = _lastPendingIndex;
	if (i >= _setSize)
		i = 0;
	do
	{
		if (_pendingData[i])
		{
			_pendingData[i] = false;
			_lastPendingIndex++;
			if (_lastPendingIndex >= _setSize)
				_lastPendingIndex = 0;
			return i;
		}

		i++;
		if (i >= _setSize)
			i = 0;
	}
	while (i != _lastPendingIndex);

	_lastPendingIndex++;
	if (_lastPendingIndex >= _setSize)
		_lastPendingIndex = 0;
	return -1;
}

NetString *TCPNetwork::GetData(int id)
{
	return &_netStrings[id];
}

int TCPNetwork::GetDataLength(int id)
{
	return _netStrings[id].BufferLength();
}

void TCPNetwork::RemoveConnection(int id)
{
	if (_listenSockets[id] == NULL)
		return;
	SDLNet_TCP_DelSocket(_set, _listenSockets[id]);
	SDLNet_TCP_Close(_listenSockets[id]);
	_listenSockets[id] = NULL;
	_freeSockets[id] = true;
	_listnerCount--;
}