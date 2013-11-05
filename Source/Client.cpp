#include "Client.h"



Client::Client() {
	pawn = new Fighter(0.0, 0.0);

	//network = NetworkFactory::getInstance();

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;

	channel_id = -1;
	player_id = -1;
	team_id = -1;

	host_port = 8080;

	// if (!network->Init())
	// 	ready = 0;
}



Client::~Client()
{
	// if (ready != 1)
 // 		return;
 	
	// network->Close();
	// ready = 2;
}



void Client::Bind(int _id) // Remove eventually!
{
	if (ready != 1)
 		return;

	channel_id = _id;
	network->Bind(channel_id);
}



bool Client::Send() {
	if (ready != 1)
 		return false;

	Parser parser;
	if (!parser.SerializeInput(inputs, NUMBER_OF_INPUTS))
		return false;
	parser.End();

	network->SendData(parser.GetStream(), channel_id);
}

NetString * Client::Receive() {
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



void Client::Update(double dt) {
	if (inputs[MOVE_FORWARD])
		pawn->force = 200.0;
	else
		pawn->force = 0.0;
	if (inputs[MOVE_BACKWARD])
		pawn->force = -200.0;
	else
		pawn->force = 0.0;
	if (inputs[TURN_LEFT])
		pawn->Turn_Left(dt);
	if (inputs[TURN_RIGHT])
		pawn->Turn_Right(dt);
	if (inputs[FIRE_ENERGY])
		pawn->Fire(0);
	if (inputs[FIRE_BALLISTIC])
		pawn->Fire(1);
	if (inputs[FIRE_MISSILE])
		pawn->Fire(2);
	if (inputs[FIRE_MINE])
		pawn->Fire(3);

	pawn->Update(dt);
}

void Client::Draw() {
	pawn->Draw();
}