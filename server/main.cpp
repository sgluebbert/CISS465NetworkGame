#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <bitset>
 
#include "SDL_net.h"
#include "./Source/Entity.h"

UDPsocket sd;       /* Socket descriptor */
UDPpacket * recieve;       /* Pointer to packet memory */
UDPpacket * send;       /* Pointer to packet memory */
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
	std::deque<Collision*> collisions;
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
					// Do something to determine new player's postion

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
				
	            entity->turn_left = inputs[0];
	            entity->turn_right = inputs[1];
	            entity->move_forward = inputs[2];
	            entity->shoot = inputs[3];
	            entity->last_input = clock();
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
					entity_buffer = new unsigned char[ENTITY_STREAM_SIZE];// + entity->used_bullets * BULLET_STREAM_SIZE];
				else
				{
					unsigned char *temp = new unsigned char[entity_buffer_index + ENTITY_STREAM_SIZE];// + entity->used_bullets * BULLET_STREAM_SIZE];
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

				// // Stream the bullets over
				// int bullets_buffer_index = entity_buffer_index + ENTITY_STREAM_SIZE;
				// for (int n = 0; n < MAX_BULLETS; ++n)
				// {
				// 	if (entity->bullets[n] == NULL)
				// 		continue;

				// 	entity_buffer[bullets_buffer_index] = BULLET;
				// 	SDLNet_Write32(entity->id, entity_buffer + bullets_buffer_index + 1);
				// 	write_float(entity->bullets[n]->x, entity_buffer + bullets_buffer_index + 5);
				// 	write_float(entity->bullets[n]->y, entity_buffer + bullets_buffer_index + 9);
				// 	bullets_buffer_index += BULLET_STREAM_SIZE;
				// }

				entity_buffer_index += ENTITY_STREAM_SIZE;// + entity->used_bullets * BULLET_STREAM_SIZE;
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

				// for (int i = 0; i < send->len; i ++)
				// 	std::cout << (int)temp[i] << ' ';
				// std::cout << '\n';
				
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

		// if (collisions.size() > 0)
		// {
		// 	int collision_buffer_index = 0;
		// 	unsigned char *collision_buffer = NULL;
		// 	for (int i = 0; i < collisions.size(); i++)
		// 	{
		// 		Collision *collision = collisions[i];
		// 		if (collision == NULL)
		// 			continue;

		// 		if (collision_buffer == NULL)
		// 			collision_buffer = new unsigned char[COLLISION_STREAM_SIZE];
		// 		else
		// 		{
		// 			unsigned char *temp = new unsigned char[collision_buffer_index + COLLISION_STREAM_SIZE];
		// 			strcpy(temp, collision_buffer, collision_buffer_index);
		// 			delete collision_buffer;
		// 			collision_buffer = temp;
		// 		}

		// 		collision_buffer[collision_buffer_index] = COLLISION;
		// 		SDLNet_Write32(collision->type, collision_buffer + collision_buffer_index + 1);
		// 		write_float(collision->x, collision_buffer + collision_buffer_index + 5);
		// 		write_float(collision->y, collision_buffer + collision_buffer_index + 9);

		// 		collision_buffer_index += COLLISION_STREAM_SIZE;
		// 		delete collisions[i]; // collisions only last one iteration
		// 		collisions[i] = NULL;
		// 	}

		// 	if (collision_buffer != NULL)
		// 	{
		// 		unsigned char *temp = new unsigned char[collision_buffer_index + 1];
		// 		strcpy(temp, collision_buffer, collision_buffer_index);
		// 		temp[collision_buffer_index] = END;
		// 		delete collision_buffer;

		// 		send->address.host = recieve->address.host;
		// 		send->address.port = recieve->address.port;
		// 		send->data = temp;
		// 	    send->len = collision_buffer_index + 1;

		// 		SDLNet_UDP_Send(sd, -1, send);
		// 	    delete temp;
		// 	}
		// }

		if (check_inactivity_timer-- == 0)
		{
			check_inactivity_timer = 10;
			clock_t diff, now = clock();
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				diff = now - entities[i]->last_input;
				if ((float)diff / CLOCKS_PER_SEC > .02)
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

			// if (collisions.size() > 0 && collisions.front() == NULL)
			// 	collisions.pop_front();
			// if (collisions.size() > 0 && collisions.back() == NULL)
			// 	collisions.pop_back();
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

    Build_Trig_Table();
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