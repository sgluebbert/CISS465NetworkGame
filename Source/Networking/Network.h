#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
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
	Network(NetworkParser *parser)
		: networkParser(parser)
	{}
	~Network()
	{
		delete networkParser;
	}

	virtual const char * get_type() const = 0;
	virtual int Init(bool isServer = false) = 0;

	virtual void Close() = 0;
	virtual bool SendData(NetString* stream, int id) = 0;
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL) = 0;
	virtual NetString *GetData(int id = 0) = 0;
	virtual int GetDataLength(int id = 0) = 0;
	virtual void RemoveConnection(int) = 0;
	virtual void Bind(int id) = 0;

	virtual IPaddress *GetIPAddress(int id = 0) = 0;

	virtual Uint32 GetRecvHost() = 0;
	virtual Uint16 GetRecvPort() = 0;
	virtual Uint32 GetSendHost() = 0;
	virtual Uint16 GetSendPort() = 0;

	bool IsInited() { return inited; }

	std::queue<NetString*> _pendingData[MaximumClients];
	int _lastPendingIndex;
	bool inited;
	NetworkParser *networkParser;
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
	UDPNetwork(NetworkParser *parser)
		: Network(parser)
	{}

	virtual const char * get_type() const { return "UDP"; }
	virtual int Init(bool isServer);
	virtual void Close();
	virtual bool SendData(NetString* stream, int id);
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL);
	virtual NetString * GetData(int id = 0);
	virtual int GetDataLength(int id = 0);
	virtual void RemoveConnection(int id);
	virtual void Bind(int id);

	virtual IPaddress *GetIPAddress(int id = 0);

	virtual Uint32 GetRecvHost() { return _recveiveip.host; }
	virtual Uint16 GetRecvPort() { return _recveiveip.port; }
	virtual Uint32 GetSendHost() { return _send->address.host; }
	virtual Uint16 GetSendPort() { return _send->address.port; }

private:
	UDPsocket _sd;
	IPaddress _recveiveip;
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
	TCPNetwork(NetworkParser *parser)
		: Network(parser)
	{}
	
	virtual const char * get_type() const { return "TCP"; }
	virtual int Init(bool isServer);
	virtual void Close();
	virtual bool SendData(NetString* stream, int id);
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL);
	virtual NetString * GetData(int id = 0);
	virtual int GetDataLength(int id = 0);
	virtual void RemoveConnection(int id);
	virtual void Bind(int id)
	{
	}

	virtual IPaddress *GetIPAddress(int id = 0);

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
	int _listnerCount;
	SDLNet_SocketSet _set;
	int _setSize;
};


//----------------------------------------------------------------------------------------------------------
// Factory
//----------------------------------------------------------------------------------------------------------
class NetworkFactory 
{
public:
	static Network* getInstance(const char *config = NULL)
	{
		switch (ChooseConnectionType(config))
		{
			case UNDEFINED:
				return NULL;
			case UDP:
				return new UDPNetwork(new NetworkParser(config));
			case TCP:
				return new TCPNetwork(new NetworkParser(config));
		}
	}

	static Network* getInstance(NetworkType type, const char *host, Uint16 host_port, Uint16 client_port = 0)
	{
		switch (ChooseConnectionType(type, host, host_port, client_port))
		{
			case UNDEFINED:
				return NULL;
			case UDP:
				return new UDPNetwork(new NetworkParser(type, host, host_port, client_port));
			case TCP:
				return new TCPNetwork(new NetworkParser(type, host, host_port, client_port));
		}
	}

private:
	NetworkFactory() {} // prevent users from creating a NetworkFactory instance.

	static NetworkType ChooseConnectionType(const char *config)
	{
		NetworkParser networkParser(config);

		std::string text = networkParser.GetNetworkType();

		if (text == "UDP")
			return UDP;
		else if (text == "TCP")
			return TCP;
		else
			throw NetworkError();
	}

	static NetworkType ChooseConnectionType(NetworkType type, const char *host, Uint16 host_port, Uint16 client_port)
	{
		NetworkParser networkParser(type, host, host_port, client_port);

		std::string text = networkParser.GetNetworkType();

		if (text == "UDP")
			return UDP;
		else if (text == "TCP")
			return TCP;
		else
			throw NetworkError();
	}
};

#endif