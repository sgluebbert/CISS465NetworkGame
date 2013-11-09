#include "Player.h"



Player::Player() {
	pawn = new Fighter(0.0, 0.0);

	network = NetworkFactory::getInstance();

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;

	channel_id = -1;
	player_id = -1;
	team_id = -1;

	host_port = 8080;

	ready = 1;
	if (!network->Init())
		ready = 0;
}



Player::~Player()
{
	if (ready != 1)
 		return;
 	
	network->Close();
	ready = 2;
}



bool Player::Send() {
	if (ready != 1)
 		return false;

	Parser parser;
	if (!parser.SerializeInput(inputs, NUMBER_OF_INPUTS))
		return false;
	parser.End();

	network->SendData(parser.GetStream(), channel_id);
}

NetString * Player::Receive() {
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


	bool reading = true;
	while (reading)
	{
		NetworkChunkEnums type;
		unsigned char temp;
		if (!netString.ReadUChar(temp))
			break;

		unsigned char playerId = 0;
		type = (NetworkChunkEnums)temp;

		switch (type)
		{
			case NCE_NEW_PLAYER:
				netString.ReadUChar(playerId);

				if (channel_id == -1)
				{
					channel_id = playerId;
					network->Bind(channel_id);
				}
				break;

			default:
				reading = false;
		}
	}


	return &netString;
}



void Player::Update(double dt) {
	if (offline) {
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
	}
	else {
		Send();
		Receive();
	}

	pawn->Update(dt);
}

void Player::Draw() {
	pawn->Draw();
}