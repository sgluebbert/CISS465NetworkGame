#include "Client.h"



Client::Client() {
	pawn = new Fighter(0.0, 0.0);

	shield_bar.Set_Rect(0, 0, 80, 20, 2);
	shield_bar.Set_Color(Color(0.00, 0.00, 0.50));

	hull_bar.Set_Rect(0, 20, 80, 20, 2);
	hull_bar.Set_Color(Color(0.50, 0.00, 0.00));

	armor_bar.Set_Rect(0, 40, 80, 20, 2);
	armor_bar.Set_Color(Color(0.00, 0.50, 0.00));

	power_bar.Set_Rect(0, 60, 80, 20, 2);
	power_bar.Set_Color(Color(0.50, 0.50, 0.00));

	shield_circle.Set_Circle(780, 20, 20, 2, 10, 0);
	shield_circle.Set_Color(Color(0.00, 0.00, 0.50));

	hull_circle.Set_Circle(780, 60, 20, 2, 10, 0);
	hull_circle.Set_Color(Color(0.50, 0.00, 0.00));

	armor_circle.Set_Circle(780, 100, 20, 2, 10, 0);
	armor_circle.Set_Color(Color(0.00, 0.50, 0.00));

	power_circle.Set_Circle(780, 140, 20, 2, 10, 0);
	power_circle.Set_Color(Color(0.50, 0.50, 0.00));

	channel_id = -1;
	player_id = -1;
	team_id = -1;
	time(&last_input);

	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
		inputs[i] = false;

	for (int i = 0; i < 5; ++i)
		fired[i] = false;

	offline = true;
}



Client::~Client()
{
	if (ready != 1)
 		return;
 	
	network->Close();
	ready = 2;
}



// bool Client::Connect() {
// 	network = NetworkFactory::getInstance();

// 	ready = 1;
// 	if (!network->Init())
// 		ready = 0;

// 	offline = 1 - ready;

// 	return ready;
// }



// bool Client::Send() {
// 	if (ready != 1)
//  		return false;

// 	Parser parser;
// 	if (!parser.SerializeInput(inputs, NUMBER_OF_INPUTS))
// 		return false;
// 	parser.End();

// 	network->SendData(parser.GetStream(), channel_id);
// }

// NetString * Client::Receive() {
// 	if (ready != 1)
//  		return NULL;
// 	// Should fix this up to do all parsing
	
// 	netString.ClearBuffer();

// 	while (network->ReceiveData() != -1)
// 	{
// 		NetString *temp = network->GetData();
// 		if (temp != NULL)
// 			netString += *temp;
// 	}

// 	netString.Seek(0);


// 	bool reading = true;
// 	while (reading)
// 	{
// 		NetworkChunkEnums type;
// 		unsigned char temp;
// 		if (!netString.ReadUChar(temp))
// 			break;

// 		unsigned char playerId = 0;
// 		type = (NetworkChunkEnums)temp;

// 		switch (type)
// 		{
// 			case NCE_NEW_PLAYER:
// 				netString.ReadUChar(playerId);

// 				if (channel_id == -1)
// 				{
// 					channel_id = playerId;
// 					network->Bind(channel_id);
// 				}
// 				break;

// 			default:
// 				reading = false;
// 		}
// 	}


// 	return &netString;
// }



void Client::Update(double dt) {
	if (offline) {
		if (inputs[MOVE_FORWARD])
			pawn->force = 200.0;
		if (inputs[MOVE_BACKWARD])
			pawn->force = -200.0;
		if (!inputs[MOVE_BACKWARD] && !inputs[MOVE_FORWARD])
			pawn->force = 0.0;
		if (inputs[TURN_LEFT])
			pawn->Turn_Left(dt);
		if (inputs[TURN_RIGHT])
			pawn->Turn_Right(dt);
		if (inputs[FIRE_ENERGY])
			fired[0] = pawn->Fire(ENERGY_TYPE);
		if (inputs[FIRE_BALLISTIC])
			fired[1] = pawn->Fire(BALLISTIC_TYPE);
		if (inputs[FIRE_PROPELLED])
			fired[2] = pawn->Fire(PROPELLED_TYPE);
		if (inputs[FIRE_MINE])
			fired[3] = pawn->Fire(BOMB_TYPE);
		if (inputs[FIRE_POWERUP])
			fired[4] = pawn->Fire(POWERUP_TYPE);
	}

	armor_bar.Notify(pawn->armor / pawn->max_armor);
	hull_bar.Notify(pawn->health / pawn->max_health);
	shield_bar.Notify(pawn->shields / pawn->max_shields);
	power_bar.Notify(pawn->power / pawn->max_power);

	armor_circle.Notify(pawn->armor / pawn->max_armor);
	hull_circle.Notify(pawn->health / pawn->max_health);
	shield_circle.Notify(pawn->shields / pawn->max_shields);
	power_circle.Notify(pawn->power / pawn->max_power);

	pawn->Update(dt);

	armor_bar.Update(dt);
	hull_bar.Update(dt);
	shield_bar.Update(dt);
	power_bar.Update(dt);

	armor_circle.Update(dt);
	hull_circle.Update(dt);
	shield_circle.Update(dt);
	power_circle.Update(dt);
}

void Client::Draw() {
	pawn->Draw();

	armor_bar.Draw();
	hull_bar.Draw();
	shield_bar.Draw();
	power_bar.Draw();

	armor_circle.Draw();
	hull_circle.Draw();
	shield_circle.Draw();
	power_circle.Draw();
}