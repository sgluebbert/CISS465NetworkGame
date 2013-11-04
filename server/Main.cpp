#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <bitset>
 
#include "../Source/Entity.h"
#include "./Source/Server.h"

void init();

int main(int argc, char **argv)
{
    Initialize_Trig_Table();
	std::deque<Entity*> entities;
	Server server;
	
	int quit = 0;
    
	while (!quit)
	{
		std::vector<int> newClients = server.HandleIncomingData();
		for (std::vector<int>::iterator it = newClients.begin(); it != newClients.end(); it++)
		{
			Entity *entity = new Entity(*it);
			entity->Respawn(entities);
			entities.push_back(entity);
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		// Update Bullets
		Bullet_List::getInstance()->Update();
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Update Players
		if (entities.size() > 0)
		{
			NetString string;
			
			for (std::deque<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				if (*it == NULL)
					continue;

				ServerClient *client = server.clientList[(*it)->id];
				if (client == NULL)
				{
					// This should never happen
					entities[it - entities.begin()] = NULL;
					delete *it;
					continue;
				}
					
				// Replace Sometime
				(*it)->turn_left = client->inputs[0];
				(*it)->turn_right = client->inputs[1];
				(*it)->move_forward = client->inputs[2];
				(*it)->shoot = client->inputs[3];
				///////////////////

				(*it)->Update(entities);

				string.WriteUChar(NCE_PLAYER);
				NetString *chunk = (*it)->Serialize();
				string += *chunk;
				delete chunk;
			}

			if (string.BufferLength() > 0)
			{
				string.WriteUChar(NCE_END);
				server.SendToAll(&string);
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////

		std::bitset<MaximumClients> removedChannels = server.CleanupClients();
		for (std::deque<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
		{
			if (*it == NULL)
				continue;
			if (removedChannels.test((*it)->id))
			{
				entities[it - entities.begin()] = NULL;
				delete *it;
			}
		}

        Clock::Frame_Control.Update();
	}
	
	return EXIT_SUCCESS;
}