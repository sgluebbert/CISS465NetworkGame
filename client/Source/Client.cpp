#include "Client.h"

Client::Client()
{
	network = NetworkFactory::getInstance();

	id = -1;
	ready = 1;
	if (!network->Init())
		ready = 0;
}

Client::~Client()
{
	if (ready != 1)
 		return;
 	
	network->Close();
	ready = 2;
}

bool Client::SendInput(bool *in)
{
	if (ready != 1)
 		return false;

	Parser parser;
	if (!parser.SerializeInput(in))
		return false;
	parser.End();

	network->SendData(parser.GetStream(), id);
}

NetString *Client::Listen()
{
	if (ready != 1)
 		return NULL;
	// Should fix this up to do all parsing
	
	netString.ClearBuffer();

	while (network->ReceiveData() != -1)
	{
		NetString *temp = network->GetData();
		if (temp != NULL)
			netString += *temp;
	}

	netString.Seek(0);
	return &netString;
}

void Client::SetId(int _id) // Remove eventually!
{
	if (ready != 1)
 		return;

	id = _id;
	network->Bind(id);
}