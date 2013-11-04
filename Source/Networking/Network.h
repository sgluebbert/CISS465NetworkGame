#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "NetworkParser.h"
#include "Parser.h"


const int MaximumClients = 32;

class NetworkError{};

//----------------------------------------------------------------------------------------------------------
// Network
//----------------------------------------------------------------------------------------------------------

class Network 
{

public:
    virtual const char * get_type() const = 0;
    virtual int Init(bool isServer = false) = 0;

    virtual void Close() = 0;
    virtual bool SendData(NetString* stream, int id) = 0;
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL) = 0;
	virtual char GetDataChar(int i) = 0;
	virtual NetString *GetData(int id = 0) = 0;
	virtual int GetDataLength(int id = 0) = 0;
	virtual void RemoveConnection(int) = 0;
	virtual void Bind(int id) = 0;

	virtual Uint32 GetRecvHost() = 0;
	virtual Uint16 GetRecvPort() = 0;
	virtual Uint32 GetSendHost() = 0;
	virtual Uint16 GetSendPort() = 0;
};

//----------------------------------------------------------------------------------------------------------
// UDP Network Class
//----------------------------------------------------------------------------------------------------------
struct Connection
{
	Connection(Uint64 _ip, Uint32 _port)
		: ip(_ip), port(_port)
	{}
	Uint64 ip;
	Uint32 port;
};

class UDPNetwork : public Network 
{

public:
    virtual const char * get_type() const { return "UDP"; }
 	
    virtual int Init(bool isServer)
    {
		_setSize = (isServer ? MaximumClients : 1);
    	
		NetworkParser * networkParser = new NetworkParser();

 		Uint16 listen_port = networkParser->GetListenPort();
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
		Uint32 host_port = networkParser->GetHostPort();

		if (SDLNet_ResolveHost(&host_address, host_ipaddress, host_port) == -1) 
		{
			fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", host_port, SDLNet_GetError());
			return -1;
		}

		for (int i = 0; i < _setSize; ++i)
			_usedSockets[i] = NULL;

		_receive->address.host = host_address.host;
		_receive->address.port = host_address.port;

		return 1;
    }

    virtual void Close()
    {
  		SDLNet_FreePacket(_receive);
		SDLNet_FreePacket(_send);
		SDLNet_Quit();
    }

 	virtual bool SendData(NetString* stream, int id)
 	{
 		if (id == -1)
		{
			_send->address.host = _receive->address.host;        /* Set the destination host */
			_send->address.port = _receive->address.port;        /* And destination port */
		}

 		_send->data = stream->Buffer();
		_send->len = stream->BufferLength();

		SDLNet_UDP_Send(_sd, id, _send);
		return true;
 	}
	
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL)
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

		// std::cout << "Received From: " << id << '\n';
		_string.ClearBuffer();
		_string.AddUChars(_receive->data, _receive->len);

		return id;
	}

	virtual char GetDataChar(int i)
	{
		// TODO: IMPLEMENT THIS
	}
	
	virtual NetString * GetData(int id = 0)
	{
		return &_string;
	}

	virtual int GetDataLength(int id = 0)
	{
		return _string.BufferLength();
	}

	virtual void RemoveConnection(int id)
	{
		delete _usedSockets[id];
		_usedSockets[id] = NULL;
	}

	virtual void Bind(int id)
	{
		SDLNet_UDP_Bind(_sd, id, &_receive->address);
	}


	virtual Uint32 GetRecvHost() { return _receive->address.host; }
	virtual Uint16 GetRecvPort() { return _receive->address.port; }
	virtual Uint32 GetSendHost() { return _send->address.host; }
	virtual Uint16 GetSendPort() { return _send->address.port; }

private:
	UDPsocket _sd;       
	UDPpacket * _receive; 
	UDPpacket * _send;
	NetString _string;
	Connection *_usedSockets[MaximumClients];
	int _setSize;
};
 

//----------------------------------------------------------------------------------------------------------
// TCP Network Class
//----------------------------------------------------------------------------------------------------------

class TCPNetwork : public Network 
{

public:
    virtual const char * get_type() const { return "TCP"; }

    virtual int Init(bool isServer)
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
 		IPaddress host_address;
		Uint32 host_port = networkParser->GetHostPort();
 		Uint16 listen_port = networkParser->GetListenPort();

		if (SDLNet_ResolveHost(&host_address, host_ipaddress, (isServer ? listen_port : host_port)) == -1) 
		{
			fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", host_port, SDLNet_GetError());
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

		return 1;
    }

    virtual void Close()
    {
    	SDLNet_TCP_Close(_sd);
		SDLNet_Quit();
    }

    virtual bool SendData(NetString* stream, int id)
 	{
 		if (_isServer)
 		{
	 		if (_listenSockets[id] == NULL)
	 			return false;

	 		if (SDLNet_TCP_Send(_listenSockets[id], stream->Buffer(), stream->BufferLength()) < stream->BufferLength())
	 			return false;
	 	}
	 	else if (SDLNet_TCP_Send(_sd, stream->Buffer(), stream->BufferLength()) < stream->BufferLength())
	 		return false;

 		return true;
 	}

	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL)
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
					SDLNet_TCP_Send(_listenSockets[freeSpot], netString.Buffer(), netString.BufferLength());
	 				_listnerCount++;
	 			}
	 			else
	 			{
	 				// Reject new socket
	 				TCPsocket temp = SDLNet_TCP_Accept(_sd);
					NetString netString;
					netString.WriteUChar(NCE_TOO_MANY_PLAYERS);
					netString.WriteUChar(NCE_END);
					SDLNet_TCP_Send(temp, netString.Buffer(), netString.BufferLength());
	 				SDLNet_TCP_Close(temp);
	 			}
	 		}
 		}

 		unsigned char buffer[1024];
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
	 				int receivedCount = SDLNet_TCP_Recv(_listenSockets[i], buffer, 1024);

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
	 					if (!_pendingData[i])
	 						_connectionStrings[i].ClearBuffer();
	 					_connectionStrings[i].AddUChars(buffer, receivedCount);
	 					_pendingData[i] = true;
	 				}
	 			}
	 		}
	 	}
	 	else
	 	{
	 		int socketReady = SDLNet_SocketReady(_sd);
	 		if (socketReady != 0)
 			{
 				int receivedCount = SDLNet_TCP_Recv(_sd, buffer, 1024);

				_connectionStrings[0].ClearBuffer();
				_connectionStrings[0].AddUChars(buffer, receivedCount);
				return 0;
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

	virtual char GetDataChar(int i)
	{
		// return _receive[i];
	}

	virtual NetString * GetData(int id = 0)
	{
		return &_connectionStrings[id];
	}

	virtual int GetDataLength(int id = 0)
	{
		return _connectionStrings[id].BufferLength();
	}

	virtual void RemoveConnection(int id)
	{
		if (_listenSockets[id] == NULL)
			return;
		SDLNet_TCP_DelSocket(_set, _listenSockets[id]);
		SDLNet_TCP_Close(_listenSockets[id]);
		_listenSockets[id] = NULL;
		_freeSockets[id] = true;
		_listnerCount--;
	}

	virtual void Bind(int id)
	{
	}

	virtual Uint32 GetRecvHost() { return _recveiveip.host; }
	virtual Uint16 GetRecvPort() { return _recveiveip.port; }
	virtual Uint32 GetSendHost() { return _sendip.host; }
	virtual Uint16 GetSendPort() { return _sendip.port; }

private:
	bool _isServer;
	IPaddress _sendip;
	IPaddress _recveiveip;
	TCPsocket _sd;
	TCPsocket _listenSockets[MaximumClients];
	bool _freeSockets[MaximumClients];
	bool _pendingData[MaximumClients];
	int _lastPendingIndex;
	NetString _connectionStrings[MaximumClients];
	int _listnerCount;
	SDLNet_SocketSet _set;
	int _setSize;
};


//----------------------------------------------------------------------------------------------------------
// Factory
//----------------------------------------------------------------------------------------------------------
enum NetworkType
{
	 UNDEFINED,
     UDP,
     TCP
};

class NetworkFactory 
{
public:
    static Network* getInstance()
    {    
    	if (instance == NULL)
    	{
	        ChooseConnectionType();

	        switch (networkType)
	        {
	        	case UNDEFINED:
	        		instance = NULL;
	        		break;
	            case UDP:
	                instance = new UDPNetwork();
	                break;
	            case TCP:
	                instance = new TCPNetwork();
	        }
	    }

        return instance;
    }

    static NetworkType getNetworkType() { return networkType; }

private:
	NetworkFactory() {} // prevent users from creating a NetworkFactory instance.

    static void ChooseConnectionType()
    {
    	NetworkParser * networkParser = new NetworkParser();

    	std::string text = networkParser->GetNetworkType();

    	if (networkParser->GetHostAddress() != NULL)
    		std::cout << "Connecting Via: " << text << " to " << networkParser->GetHostAddress() << ":" << networkParser->GetHostPort() << std::endl;
    	else
    		std::cout << "Connecting Via: " << text << "; Listening on port " << networkParser->GetListenPort() << std::endl;

        if (text == "UDP") 
        {
            networkType = UDP;
        }
        else if (text == "TCP")
        {
            networkType = TCP;
        }
        else
        {
            throw NetworkError();
        }
    }

    static Network *instance;
    static NetworkType networkType;
};

#endif