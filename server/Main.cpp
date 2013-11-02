#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <bitset>
 
#include "../Source/Networking/Parser.h"
#include "../Source/Entity.h"
#include "../Source/Networking/Network.h"

Network* NetworkFactory::instance = NULL;
NetworkType NetworkFactory::networkType = UNDEFINED;
Network *network = NetworkFactory::getInstance();

void init();
int get_next_free(std::bitset<32> &);
inline void strcpy(unsigned char *dst, unsigned char *src, int size);
inline void write_float(float value, unsigned char *buffer);
	
const int ENTITY_STREAM_SIZE = 19;
const int BULLET_STREAM_SIZE = 13;
const int COLLISION_STREAM_SIZE = 13;

int main(int argc, char **argv)
{
    char * buffer[255];

	std::deque<Entity*> entities;
	std::bitset<32> taken_ids;
    
	Uint8 inputs[4];
	
	int quit;
 
	init();
	NetString netString;
    
	/* Main loop */
	quit = 0;
	int check_inactivity_timer = 100;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		while (network->ReceiveData())
		{		
			bool exists = false;
			Entity *entity;
			int entity_index = 0;

			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				if (entities[i]->ip == network->GetRecvHost() && entities[i]->port == network->GetRecvPort())
				{
					entity = entities[i];
					exists = true;
					entity_index = i;
					break;
				}
			}

			if (!exists)
			{
				int next_player_id = get_next_free(taken_ids);
				if (next_player_id != -1)
				{
					entity = new Entity(network->GetRecvHost(), network->GetRecvPort(), next_player_id);
					entities.push_back(entity);
					entity->last_input = clock();
					int spawn_point = GetSpawnPoint(entities);
					entity->x = spawn_points[spawn_point][0];
					entity->y = spawn_points[spawn_point][1];

					netString.ClearBuffer();
					netString.WriteUChar(NCE_NEW_PLAYER);
					netString.WriteUChar(next_player_id);
					netString.WriteUChar(NCE_END);

					network->SendData(netString.Buffer(), -1, netString.BufferLength());
					network->Bind(next_player_id);

					std::cout << "New Player Joined\n";
					std::cout << "    id: " << next_player_id << '\n';
					taken_ids[next_player_id] = true;
				}
			}
			else if (entity != NULL)
			{
				for (int i = 0; i < 4; i++)
				    inputs[i] = network->GetDataChar(i);
					
				if (entity->health > 0) {
					entity->turn_left = inputs[0];
					entity->turn_right = inputs[1];
					entity->move_forward = inputs[2];
					entity->shoot = inputs[3];
				}
				
	            time(&entity->last_input);

	            if (entity->health == 0) {
            		if (entity->death_time == 0)
            			time(&entity->death_time);
            		else {
            			time_t now;
            			time(&now);
            			if (now - entity->death_time >= 3) {
							int spawn_point = GetSpawnPoint(entities);
							entity->x = spawn_points[spawn_point][0];
							entity->y = spawn_points[spawn_point][1];
							entity->health = 100;
							entity->death_time = 0;
							entity->velocity = 0;
						}
            		}
            	}

			}
		}

		Bullet_List::getInstance()->Update();

		if (entities.size() > 0)
		{
			netString.ClearBuffer();
			for (int i = 0; i < entities.size(); i++)
			{
				Entity *entity = entities[i];
				if (entity == NULL)
					continue;
	                
	            entity->Update();

	            netString.WriteUChar(NCE_PLAYER);
	            netString.WriteUChar(entity->id);
	            netString.WriteFloat(entity->x);
	            netString.WriteFloat(entity->y);
	            netString.WriteFloat(entity->angle);
	            netString.WriteFloat(entity->health);
	            netString.WriteBool(entity->did_shoot);
			}

			if (netString.BufferLength() > 0)
			{
	            netString.WriteUChar(NCE_END);

				for (int i = 0; i < entities.size(); i++)
				{
					Entity *entity = entities[i];
					if (entity == NULL)
						continue;
					network->SendData(netString.Buffer(), entity->id, netString.BufferLength());

				}
			}
		}

		if (check_inactivity_timer-- == 0)
		{
			check_inactivity_timer = 2000;
			time_t now;
			time(&now);
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				if (now - entities[i]->last_input > 5)
				{
					std::cout << "Kicking player " << entities[i]->id << " because of inactivity.\n";

					netString.ClearBuffer();
					netString.WriteUChar(NCE_REMOVE_PLAYER);
					netString.WriteUChar(entities[i]->id);
					netString.WriteUChar(NCE_END);

					for (int n = 0; n < entities.size(); n++)
					{
						Entity *entity = entities[n];
						if (entity == NULL)
							continue;
						network->SendData(netString.Buffer(), entity->id, netString.BufferLength());
					}

					taken_ids[entities[i]->id] = false;
					delete entities[i];
					entities[i] = NULL;
				}
			}

			if (entities.size() > 0 && entities.front() == NULL)
				entities.pop_front();
			if (entities.size() > 0 && entities.back() == NULL)
				entities.pop_back();
		}

        Clock::Frame_Control.Update();
	}
 
	network->Close();
	SDLNet_Quit();
	return EXIT_SUCCESS;
}

inline void write_float(float value, unsigned char *buffer)
{
	unsigned int temp;
	memcpy(&temp, &value, min(sizeof(float), sizeof(unsigned int)));
	SDLNet_Write32(temp, buffer);
}

void init()
{
	network->InitServer();

    Initialize_Trig_Table();
}

int get_next_free(std::bitset<32> &taken_ids)
{
	for (int i = 0; i < 32; i++)
	{
		if (!taken_ids.test(i))
			return i;
	}

	return -1;
}

inline void strcpy(unsigned char *dst, unsigned char *src, int size)
{
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
}