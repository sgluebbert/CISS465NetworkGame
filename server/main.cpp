#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
 
#include "SDL_net.h"
#include "./Source/Entity.h"
 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket * recieve;       /* Pointer to packet memory */
	UDPpacket * send;       /* Pointer to packet memory */
    char * buffer[255];
    int num_of_channels = -1;
    
	Uint16 server_port = 8080;

	std::vector<Entity*> entities;
    
	Uint8 inputs[4];
	
	int quit;
 
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
 
    Build_Trig_Table();
    
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		while (SDLNet_UDP_Recv(sd, recieve))
		{
			bool exists = false;
			int entityIndex = -1;
			Entity *entity;
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->team == recieve->channel)
				{
					exists = true;
					entityIndex = i;
					break;
				}
			}

			if (!exists)
			{
				std::cout << "New player joined on channel: " << recieve->channel << '\n';
				entity = new Entity(recieve->channel);
				entities.push_back(entity);
				entity->max_velocity = 50;
				entity->acceleration = 8;
				entity->deceleration = 6;
				entity->turn_rate = 30;
				// Do something to determine new player's postion
				entityIndex = entities.size() - 1;

				// Ignore first frame given to us
				continue;
			}
			else
				entity = entities[entityIndex];

			if (entity == NULL)
				continue;

	        /*If a player sends a setup packet*/
		    /*if (p->channel == -1) {
		        num_of_channels += 1;
		        p->data = num_of_channels;
		    }*/
	        /*If a player sends a setup packet*/
		    //else if (p->chan <= num_of_channels)
			for (int i = 0; i < 4; i++)
			    inputs[i] = recieve->data[i];
			
            entity->turn_left = inputs[0];
            entity->turn_right = inputs[1];
            entity->move_forward = inputs[2];
            entity->shoot = inputs[3];
                
            entity->Update();
		}		
		
		if (entities.size() > 0)
		{
			std::stringstream outbound_stream;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity *entity = entities[i];
				if (entity == NULL)
					continue;

				outbound_stream << entity->team << ' ' << entity->x << ' ' << entity->y << ' ' << entity->angle;
			}

			const char * outbound_data = outbound_stream.str().c_str();
			send->address.host = recieve->address.host;
			send->address.port = recieve->address.port;
			send->data = (unsigned char *) outbound_data;
		    send->len = strlen((char *)send->data) + 1;
			
			// Does this send to all clients? if no we need to loop again sending the stream.
			SDLNet_UDP_Send(sd, -1, send);
		}

        SDL_Delay(50);
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(recieve);
	SDLNet_FreePacket(send);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
