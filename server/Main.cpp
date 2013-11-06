#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <bitset>
 
#include "../Source/Ship.h"
#include "./Source/Server.h"

void init();

int main(int argc, char **argv)
{
    Initialize_Trig_Table();
	Ship *ships[MaximumClients];
	Server server;

	for (int i = 0; i < MaximumClients; ++i)
		ships[i] = NULL;
	
	int quit = 0;
    
	while (!quit)
	{
		std::vector<int> newClients = server.HandleIncomingData();
		for (std::vector<int>::iterator it = newClients.begin(); it != newClients.end(); it++)
		{
			Ship *ship = new Ship();
			// entity->Respawn(entities);
			ships[*it] = ship;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		// Update Bullets
		// Bullet_List::getInstance()->Update();
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Update Players
		NetString string;
		
		Ship *ship;
		double dt = Clock::Frame_Control.Get_Time_Per_Frame();
		for (int i = 0; i < MaximumClients; ++i)
		{
			ship = ships[i];
			if (ship == NULL)
				continue;

			ServerClient *client = server.clientList[i];
			if (client == NULL)
			{
				// This should never happen
				ships[i] = NULL;
				delete ship;
				continue;
			}
				
			// Replace Sometime
			if (client->inputs[MOVE_FORWARD])
				ship->force = 200.0;
			else
				ship->force = 0.0;
			if (client->inputs[MOVE_BACKWARD])
				ship->force = -200.0;
			else
				ship->force = 0.0;
			if (client->inputs[TURN_LEFT])
				ship->Turn_Left(dt);
			if (client->inputs[TURN_RIGHT])
				ship->Turn_Right(dt);
			if (client->inputs[FIRE_ENERGY])
				ship->Fire(0);
			///////////////////

			ship->Update(dt);

			string.WriteUChar(NCE_PLAYER);
			NetString *chunk = ship->Serialize();
			string += *chunk;
			delete chunk;
		}

		if (string.BufferLength() > 0)
		{
			string.WriteUChar(NCE_END);
			server.SendToAll(&string);
		}
		///////////////////////////////////////////////////////////////////////////////////////////

		std::bitset<MaximumClients> removedChannels = server.CleanupClients();
		for (int i = 0; i < MaximumClients; ++i)
		{
			ship = ships[i];
			if (ship == NULL)
				continue;
			if (removedChannels.test(i))
			{
				ships[i] = NULL;
				delete ship;
			}
		}

        Clock::Frame_Control.Update();
	}
	
	return EXIT_SUCCESS;
}