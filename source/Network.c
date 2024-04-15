#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include "Network.h"

void initNetwork_Client( UDPsocket* sd, IPaddress* srvadd )
{

  if (SDLNet_Init() <0)
    {
      fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (!(p = SDLNet_AllocPacket(512)))
    {
      fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (!(pReceive= SDLNet_AllocPacket(512)))
      {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
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
  if (!(p = SDLNet_AllocPacket(512)))
    {
      fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  if (!(pReceive= SDLNet_AllocPacket(512)))
      {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
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
    SDLNet_FreePacket(pReceive);
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}
void serverSendPacket(udpDataToClient Data, IPaddress* destAddr, UDPsocket* sd)
{
  memcpy(p->data,&Data,sizeof(udpDataToClient));
  p->address.host = destAddr->host;
  p->address.port = destAddr->port;	/* And destination port */
  p->len =sizeof(udpDataToClient);
  SDLNet_UDP_Send(*sd, -1, p);
}
IPaddress serverReceivePacket(udpDataToServer* packet, UDPsocket* sd)
  {
    if(SDLNet_UDP_Recv(*sd,pReceive))
      {
        memcpy(packet,pReceive->data,sizeof(udpDataToServer));
        return pReceive->address;
      }
    IPaddress empty = {0,0};
    return empty;
  }

void clientSendPacket(udpDataToServer Data, IPaddress* destAddr, UDPsocket* sd)
  {
    memcpy(p->data,&Data,sizeof(udpDataToServer));
    p->address.host = destAddr->host;
    p->address.port = destAddr->port;	/* And destination port */
    p->len =sizeof(udpDataToServer);
    SDLNet_UDP_Send(*sd, -1, p);
  }

IPaddress clientReceivePacket(udpDataToClient* packet, UDPsocket* sd)
  {
    if(SDLNet_UDP_Recv(*sd,pReceive))
      {
        memcpy(packet,pReceive->data,sizeof(udpDataToClient));
        return pReceive->address;
      }
    IPaddress empty = {0,0};
    return empty;
  }
void closeNetwork_Client(UDPsocket* sd)
{
    SDLNet_UDP_Close(*sd);
    SDLNet_FreePacket(pReceive);
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}
