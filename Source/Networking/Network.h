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
	virtual NetString *GetData(int id = 0) = 0;
	virtual int GetDataLength(int id = 0) = 0;
	virtual void RemoveConnection(int) = 0;
	virtual void Bind(int id) = 0;

	virtual Uint32 GetRecvHost() = 0;
	virtual Uint16 GetRecvPort() = 0;
	virtual Uint32 GetSendHost() = 0;
	virtual Uint16 GetSendPort() = 0;

	NetString _netStrings[MaximumClients];
	bool _pendingData[MaximumClients];
	int _lastPendingIndex;
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
    virtual int Init(bool isServer);
    virtual void Close();
 	virtual bool SendData(NetString* stream, int id);
	virtual int ReceiveData(std::vector<int> *newClients = NULL, std::vector<int> *removedClients = NULL);
	virtual NetString * GetData(int id = 0);
	virtual int GetDataLength(int id = 0);
	virtual void RemoveConnection(int id);
	virtual void Bind(int id);

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