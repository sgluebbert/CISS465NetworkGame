#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <bitset>
 
#include "SDL_net.h"
#include "../Source/Entity.h"

UDPsocket sd;       /* Socket descriptor */
// UDPpacket * recieve;       /* Pointer to packet memory */
// UDPpacket * send;       /* Pointer to packet memory */
Uint16 server_port = 8080;
void init();
int get_next_free(std::bitset<32> &);
inline void strcpy(unsigned char *dst, unsigned char *src, int size);
inline void write_float(float value, unsigned char *buffer);

enum NetworkType { END, NEW_PLAYER, PLAYER, REMOVE_PLAYER, BULLET, COLLISION };
		
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
    
	/* Main loop */
	quit = 0;
	int check_inactivity_timer = 100;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		while (SDLNet_UDP_Recv(sd, recieve))
		{
			bool exists = false;
			Entity *entity;
			int entity_index = 0;

			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				if (entities[i]->ip == recieve->address.host && entities[i]->port == recieve->address.port)
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
					entity = new Entity(recieve->address.host, recieve->address.port, next_player_id);
					entities.push_back(entity);
					entity->last_input = clock();
					int spawn_point = GetSpawnPoint(entities);
					entity->x = spawn_points[spawn_point][0];
					entity->y = spawn_points[spawn_point][1];

					unsigned char buffer[3];
					buffer[0] = NEW_PLAYER;
					buffer[1] = next_player_id;
					buffer[2] = END;

					send->address.host = recieve->address.host;
					send->address.port = recieve->address.port;
					send->data = buffer;
				    send->len = 3;

					SDLNet_UDP_Send(sd, -1, send);
	       			SDLNet_UDP_Bind(sd, next_player_id, &recieve->address);

					std::cout << "New Player Joined\n";
					std::cout << "    id: " << next_player_id << '\n';
					taken_ids[next_player_id] = true;
				}
			}
			else if (entity != NULL)
			{
				for (int i = 0; i < 4; i++)
				    inputs[i] = recieve->data[i];
					
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
			int entity_buffer_index = 0;
			unsigned char *entity_buffer = NULL;
			for (int i = 0; i < entities.size(); i++)
			{
				Entity *entity = entities[i];
				if (entity == NULL)
					continue;
	                
	            entity->Update();

				if (entity_buffer == NULL)
					entity_buffer = new unsigned char[ENTITY_STREAM_SIZE];
				else
				{
					unsigned char *temp = new unsigned char[entity_buffer_index + ENTITY_STREAM_SIZE];
					strcpy(temp, entity_buffer, entity_buffer_index);
					delete entity_buffer;
					entity_buffer = temp;
				}

				entity_buffer[entity_buffer_index] = PLAYER;
				entity_buffer[entity_buffer_index + 1] = entity->id;
				write_float(entity->x, entity_buffer + entity_buffer_index + 2);
				write_float(entity->y, entity_buffer + entity_buffer_index + 6);
				write_float(entity->angle, entity_buffer + entity_buffer_index + 10);
				write_float(entity->health, entity_buffer + entity_buffer_index + 14);
				entity_buffer[entity_buffer_index + 18] = entity->did_shoot;

				entity_buffer_index += ENTITY_STREAM_SIZE;
			}

			if (entity_buffer != NULL)
			{
				unsigned char *temp = new unsigned char[entity_buffer_index + 1];
				strcpy(temp, entity_buffer, entity_buffer_index);
				temp[entity_buffer_index] = END;
				delete entity_buffer;

				send->address.host = recieve->address.host;
				send->address.port = recieve->address.port;
				send->data = temp;
			    send->len = entity_buffer_index + 1;
				
				for (int i = 0; i < entities.size(); i++)
				{
					Entity *entity = entities[i];
					if (entity == NULL)
						continue;
					SDLNet_UDP_Send(sd, entity->id, send);
				}
			    delete temp;
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

					unsigned char buffer[6];
					buffer[0] = REMOVE_PLAYER;
					buffer[1] = entities[i]->id;
					buffer[2] = END;

					send->address.host = recieve->address.host;
					send->address.port = recieve->address.port;
					send->data = buffer;
					send->len = 3;
					SDLNet_UDP_Send(sd, -1, send);

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

        Timer::Frame_Control.Update();
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(recieve);
	SDLNet_FreePacket(send);
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
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(server_port)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
	if (!(recieve = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
	if (!(send = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	std::cout << "Server listening on port " << server_port << '\n';

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