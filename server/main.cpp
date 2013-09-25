#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL_net.h"
#include "./Source/System.h"
#include "./Source/Entity.h"
 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket * recieve;       /* Pointer to packet memory */
	UDPpacket * send;       /* Pointer to packet memory */
    char * buffer[255];
    int num_of_channels = -1;
    
	Uint16 server_port = 8080;
	
	Entity player;
    player.max_velocity = 50;
    player.acceleration = 8;
    player.deceleration = 6;
    player.turn_rate = 30;
    
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
	        /*If a player sends a setup packet*/
		    /*if (p->channel == -1) {
		        num_of_channels += 1;
		        p->data = num_of_channels;
		    }*/
	        /*If a player sends a setup packet*/
		    //else if (p->chan <= num_of_channels)
			for (int i = 0; i < 4; i++)
			    inputs[i] = recieve->data[i];
			
            player.turn_left = inputs[0];
            player.turn_right = inputs[1];
            player.move_forward = inputs[2];
            
            if (inputs[3])
                player.TryFire();
                
            player.Update();
		}		
		
		std::stringstream outbound_stream;
		outbound_stream << player.x << ' ' << player.y << ' ' << player.angle;
		const char * outbound_data = outbound_stream.str().c_str();
		send->address.host = recieve->address.host;
		send->address.port = recieve->address.port;
		send->data = (unsigned char *) outbound_data;
	    send->len = strlen((char *)send->data) + 1;
		
		SDLNet_UDP_Send(sd, -1, send);

        SDL_Delay(50);
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(recieve);
	SDLNet_FreePacket(send);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
