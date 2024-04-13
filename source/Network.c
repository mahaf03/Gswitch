#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_net.h>

void initNetwork(UDPsocket *sd, IPaddress *srvadd, const char *host, Uint16 port)
{
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDLNet_ResolveHost(srvadd, host, port) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    *sd = SDLNet_UDP_Open(port);
    if (!*sd)
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void sendData(UDPsocket sd, IPaddress srvadd, const char *message)
{
    UDPpacket *packet = SDLNet_AllocPacket(strlen(message) + 1);
    if (!packet)
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    packet->address = srvadd;
    packet->len = strlen(message) + 1;
    strcpy((char *)packet->data, message);

    if (SDLNet_UDP_Send(sd, -1, packet) == 0)
    {
        fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
    }

    SDLNet_FreePacket(packet);
}

void receiveData(UDPsocket sd, UDPpacket *packet)
{
    if (SDLNet_UDP_Recv(sd, packet))
    {
        printf("Received: %s\n", (char *)packet->data);
    }
}

void closeNetwork(UDPsocket sd)
{
    SDLNet_UDP_Close(sd);
    SDLNet_Quit();
}
