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

			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				if (entities[i]->ip == recieve->address.host && entities[i]->port == recieve->address.port)
				{
					entity = entities[i];
					exists = true;
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

					std::stringstream outbound_stream;
					outbound_stream << "N " << next_player_id << " # ";
					const char * outbound_data = outbound_stream.str().c_str();
					send->address.host = recieve->address.host;
					send->address.port = recieve->address.port;
					send->data = (unsigned char *) outbound_data;
				    send->len = strlen((char *)send->data) + 1;

					SDLNet_UDP_Send(sd, -1, send);

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
	                
	            entity->Update();
			}
		}		
		
		if (entities.size() > 0)
		{
			std::stringstream outbound_stream;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity *entity = entities[i];
				if (entity == NULL)
					continue;

				outbound_stream << "P " << entity->id << ' ' << entity->x << ' ' << entity->y << ' ' << entity->angle << ' ';

				for (int n = 0; n < MAX_BULLETS; ++n)
				{
					if (entity->bullets[n] == NULL)
						continue;

					outbound_stream << "B " << entity->id << ' ' << entity->bullets[n]->x << ' ' << entity->bullets[n]->y << ' ';
				}
			}

			outbound_stream << "# ";
			const char * outbound_data = outbound_stream.str().c_str();
			send->address.host = recieve->address.host;
			send->address.port = recieve->address.port;
			send->data = (unsigned char *) outbound_data;
		    send->len = strlen((char *)send->data) + 1;
			
			SDLNet_UDP_Send(sd, -1, send);
		}

		if (check_inactivity_timer-- == 0)
		{
			check_inactivity_timer = 100;
			clock_t diff, now = clock();
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i] == NULL)
					continue;

				diff = now - entities[i]->last_input;
				if (diff > 50000)
				{
					std::cout << "Kicking player " << entities[i]->id << " because of inactivity.\n";

					std::stringstream outbound_stream;
					outbound_stream << "R " << entities[i]->id << " # ";
					const char * outbound_data = outbound_stream.str().c_str();
					send->address.host = recieve->address.host;
					send->address.port = recieve->address.port;
					send->data = (unsigned char *) outbound_data;
				    send->len = strlen((char *)send->data) + 1;
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

        SDL_Delay(50);
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(recieve);
	SDLNet_FreePacket(send);
	SDLNet_Quit();
	return EXIT_SUCCESS;
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