#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "Network.h"

int main(int argc,char** argv)
  {
    UDPsocket sd;
    UDPpacket* pSent;
    printf("opening server..\n");

    initNetwork_Server (&sd);

	//if (!((pSent = SDLNet_AllocPacket(512))&&(pReceive= SDLNet_AllocPacket(512))))
	if (!((pSent = SDLNet_AllocPacket(512))))
      {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
 	}

    struct udpData message;
    int quit = 0;
    while (quit==0)
      {
        if(receivePacket(&message,&sd) ==1)
          {
            printf("New packet received!\n\t %x %x %x \n",message.xPos,message.yPos,message.status);
            if (message.status == 3)
              {quit = 1;}
          }
        //SDL_Delay(1000);
      }
    closeNetwork_Server(&sd,pSent);
    printf("server closed..\n");
    return 0;
  }
