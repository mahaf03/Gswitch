#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "Network.h"

int main(int argc,char** argv)
  {
    UDPsocket sd;
    UDPpacket *pReceive;
    UDPpacket* pSent;
    printf("opening server..\n");
    initNetwork_Server (&sd,pReceive,pSent);

    printf("closing server..\n");
    //sd = SDLNet_UDP_Open(NETWORK_PORT);
    struct udpData message;
    for (int i= 0;i<10;i++)
      {
        if(receivePacket(&message,&sd,pReceive) ==1)
          printf("New packet received!\n");
        SDL_Delay(1000);
      }
    closeNetwork_Server(&sd,pReceive,pSent);
    printf("server closed..\n");
    return 0;
  }
