#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include "Network.h"

void initNetwork_Client( UDPsocket* sd, IPaddress* srvadd, UDPpacket *p, UDPpacket* pReceive)
{
  if (SDLNet_Init() <0)
    {
      fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  if (!(*sd = SDLNet_UDP_Open(0)))
    {
      fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (SDLNet_ResolveHost(srvadd, SERVERADDRESS, 49156) == -1)
    {
      fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (!(p = SDLNet_AllocPacket(512)))
    {
      fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (!((pReceive= SDLNet_AllocPacket(512))))
    {
      fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
}

void initNetwork_Server(UDPsocket* sd, UDPpacket* pReceive,UDPpacket* pSent)
{

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	if (!(*sd = SDLNet_UDP_Open(NETWORK_PORT)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	if (!((pSent = SDLNet_AllocPacket(512))&&(pReceive= SDLNet_AllocPacket(512))))
      {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
 	}

}

void closeNetwork_Server(UDPsocket* sd, UDPpacket* pSent, UDPpacket* pReceive)
{

    SDLNet_UDP_Close(*sd);
 	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pReceive);
	SDLNet_Quit();
}

int receivePacket(struct udpData* packet, UDPsocket* sd, UDPpacket* pReceive)
  {
    if(SDLNet_UDP_Recv(*sd,pReceive))
      {
        printf("packet received from host: \n\t %x\n",pReceive->address.host);
        memcpy(packet,pReceive->data,sizeof(struct udpData));
        return 1;
      }
    else
      return 0;
  }

void sendPacket(UDPpacket* p, IPaddress* destAddr, UDPsocket* sd)
  {
    printf("sending packet!\n");
    p->data = "1 2 3\n";

    printf("data set!\n");
    //sprintf((char*)p->data,"%d %d %d\n", (uint)1,(int)2,(int)3);
    p->address.host = destAddr->host;
    p->address.port = destAddr->port;	/* And destination port */
    //printf("datalengnth %d \n",strlen((char *)p->data) + 1);
    //p->len = strlen((char *)p->data) + 1;
    printf("stringlength set \n");
    SDLNet_UDP_Send(*sd, -1, p);

    printf("packet sent!!\n");
  }
