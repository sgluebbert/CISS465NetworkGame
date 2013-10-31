#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_net.h>


class NetworkError{};

//----------------------------------------------------------------------------------------------------------
// Network
//----------------------------------------------------------------------------------------------------------

class Network 
{

public:
    virtual const char * get_type() const = 0;
    virtual int Init(Uint16 receive_port) = 0;
    virtual int InitClient() = 0;
    virtual int InitServer() = 0;

    virtual void Close() = 0;
    virtual void SendData(unsigned char * buffer, int id, int len) = 0;
	virtual int ReceiveData() = 0;
	virtual void Bind(int next_player_id) = 0;
	virtual char GetDataChar(int i) = 0;
	virtual unsigned char * GetData() = 0;
	virtual int GetDataLength() = 0;

	virtual Uint32 GetRecvHost() = 0;
	virtual Uint16 GetRecvPort() = 0;
	virtual Uint32 GetSendHost() = 0;
	virtual Uint16 GetSendPort() = 0;
};

//----------------------------------------------------------------------------------------------------------
// UDP Network Class
//----------------------------------------------------------------------------------------------------------

class UDPNetwork : public Network 
{

public:
    virtual const char * get_type() const { return "UDP"; }
 	
    virtual int Init(Uint16 receive_port)
    {
    	if (!(_sd = SDLNet_UDP_Open(receive_port)))
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

		return 1;
    }

 	virtual int InitClient()
 	{
 		Uint16 receive_port = 0;

 		Init(receive_port);

 		IPaddress server_address;
		const char * server_ipaddress = "localhost";
		Uint32 server_port = 1236;

		if (SDLNet_ResolveHost(&server_address, server_ipaddress, server_port) == -1) 
		{
			fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", server_port, SDLNet_GetError());
			return -1;
		}

		_receive->address.host = server_address.host;
		_receive->address.port = server_address.port;

 	}

    virtual int InitServer()
    {
    	Uint16 receive_port = 1236;
		Init(receive_port);


		IPaddress client_address;
		const char * client_ipaddress = NULL;
		Uint32 client_port = 0;

		if (SDLNet_ResolveHost(&client_address, client_ipaddress, client_port) == -1) 
		{
			fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", client_port, SDLNet_GetError());
			return -1;
		}

		_receive->address.host = client_address.host;
		_receive->address.port = client_address.port;

    }

    virtual void Close()
    {
  		SDLNet_FreePacket(_receive);
		SDLNet_FreePacket(_send);
		SDLNet_Quit();
    }

 	virtual void SendData(unsigned char * buffer, int id, int len)
 	{
 		if (id == -1)
 		{
 			_send->address.host = _receive->address.host;	/* Set the destination host */
    		_send->address.port = _receive->address.port;	/* And destination port */
 		}
 		
		_send->data = buffer;
		_send->len = len;

		SDLNet_UDP_Send(_sd, id, _send);
 	}
	
	virtual int ReceiveData()
	{
		return SDLNet_UDP_Recv(_sd, _receive);
	}

	virtual void Bind(int next_player_id)
	{
		SDLNet_UDP_Bind(_sd, next_player_id, &_receive->address);
	}

	virtual char GetDataChar(int i)
	{
		return _receive->data[i];
	}
	
	virtual unsigned char * GetData()
	{
		return _receive->data;
	}

	virtual int GetDataLength()
	{
		return _receive->len;
	}


	virtual Uint32 GetRecvHost() { return _receive->address.host; }
	virtual Uint16 GetRecvPort() { return _receive->address.port; }
	virtual Uint32 GetSendHost() { return _send->address.host; }
	virtual Uint16 GetSendPort() { return _send->address.port; }

private:
	UDPsocket _sd;       
	UDPpacket * _receive; 
	UDPpacket * _send;    
};
 

//----------------------------------------------------------------------------------------------------------
// TCP Network Class
//----------------------------------------------------------------------------------------------------------

class TCPNetwork : public Network 
{

public:
    virtual const char * get_type() const { return "TCP"; }
 	
 	virtual int InitServer()
 	{

 	}

 	virtual int InitClient()
 	{

 	}

    virtual int Init(Uint16 receive_port)
    {
  		// TODO: IMPLEMENT THIS
    }

    virtual void Close()
    {
  		// SDLNet_TCP_Close(_sd);
		// SDLNet_Quit();
    }

    virtual void SendData(unsigned char * buffer, int id, int len)
 	{
 		//SDLNet_TCP_Send(_sd, buffer, len);
 	}

	virtual int ReceiveData()
	{
		// int result = SDLNet_TCP_Recv(_sd, _receive, 512);

		// if (result <= 0)
		//	return 0;

		// return 1;
	}

	virtual void Bind(int next_player_id)
	{
		// TODO: IMPLEMENT THIS
	}

	virtual char GetDataChar(int i)
	{
		// return _receive[i];
	}

	virtual unsigned char * GetData()
	{
		// return _receive;
	}

	virtual int GetDataLength()
	{
		// TODO: IMPLEMENT THIS
	}

	virtual Uint32 GetRecvHost() { return _recveiveip.host; }
	virtual Uint16 GetRecvPort() { return _recveiveip.port; }
	virtual Uint32 GetSendHost() { return _sendip.host; }
	virtual Uint16 GetSendPort() { return _sendip.port; }

private:
	IPaddress _sendip;
	IPaddress _recveiveip;
	TCPsocket _sd;
	char * _receive;
	char * _send;
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
        std::string text;
        std::ifstream config_file("../Source/conf/network.conf");
        if (config_file.is_open())
            std::getline(config_file, text);

        config_file.close();

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