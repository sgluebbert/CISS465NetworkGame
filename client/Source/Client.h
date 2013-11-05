#ifndef CLIENT_H
#define CLIENT_H

#include "../../Source/Networking/Network.h"

enum DebugLevel { DL_NONE, DL_LOW, DL_MED, DL_HIGH };

struct Client
{
	Client();
	~Client();
	
	bool SendInput(bool *);
	NetString *Listen();

	int GetReadyState() { return ready; }
	int GetId() { return id; }

	void SetId(int _id); // Remove eventually!
private:
	Network *network;
	NetString netString;
	int ready;
	int id;
};

#endif