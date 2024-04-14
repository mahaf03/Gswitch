#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include "Network.h"

void initNetwork_Client( UDPsocket* sd, IPaddress* srvadd, UDPpacket* pReceive)
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
}

void initNetwork_Server(UDPsocket* sd)
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

}

void closeNetwork_Server(UDPsocket* sd, UDPpacket* pSent)
{
    SDLNet_UDP_Close(*sd);
 	SDLNet_FreePacket(pSent);
	SDLNet_Quit();
}

int receivePacket(struct udpData* packet, UDPsocket* sd)
  {
    UDPpacket* pReceive;
	if (!(pReceive= SDLNet_AllocPacket(512)))
      {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
 	}
    if(SDLNet_UDP_Recv(*sd,pReceive))
      {
        printf("packet received from host: \n\t %x\n",pReceive->address.host);

        memcpy(packet,pReceive->data,sizeof(struct udpData));
        SDLNet_FreePacket(pReceive);
        return 1;
      }
    else
        SDLNet_FreePacket(pReceive);
      return 0;
  }

void sendPacket(struct udpData Data, IPaddress* destAddr, UDPsocket* sd)
//void sendPacket(UDPpacket* p, IPaddress* destAddr, UDPsocket* sd)
  {
    printf("data set!\n");
    UDPpacket* p;
    if (!(p = SDLNet_AllocPacket(512)))
        {
            fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    //sprintf((char*)p->data,"%d %d %d\n",Data.xPos,Data.yPos,Data.status);
    memcpy(p->data,&Data,sizeof(struct udpData));
    //sprintf((char*)p->data,"%d %d %d\n", (uint)1,(int)2,(int)3);
    p->address.host = destAddr->host;
    p->address.port = destAddr->port;	/* And destination port */
    p->len =sizeof(struct udpData);
    SDLNet_UDP_Send(*sd, -1, p);
    printf("packet sent!!\n");
    SDLNet_FreePacket(p);
  }
