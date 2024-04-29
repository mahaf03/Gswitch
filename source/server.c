#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "Network.h"
#define MAX_PLAYERS 4;

int main(int argc, char **argv)
{
  UDPsocket sd;
  UDPpacket *pSent;
  Uint32 frameStart;
  printf("opening server..\n");
  udpDataToClient dataSend = {};
  dataSend.status = 0;
  IPaddress players[MAX_PLAYERS];
  int playercount = 0;
  for (int i = 0; i < 4; i++)
  {
    IPaddress empty = {0, 0};
    players[i] = empty;
  }
  initNetwork_Server(&sd);
  if (!((pSent = SDLNet_AllocPacket(512))))
  {
    fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  udpDataToServer message;
  int quit = 0;
  while (quit == 0)
  {
    IPaddress host;
    host = serverReceivePacket(&message, &sd);
    int gotPkg = (host.host != 0 && host.port != 0);

    if (gotPkg)
      {
        int playerNo = -1;
        for (int i = 0; i < 4; i++)
          {
            if ((players[i].host == 0 && players[i].port == 0) || (host.host == players[i].host && host.port == players[i].port))
              {
                // We got a package from either a new player or player i
                if (players[i].host == 0 && players[i].port == 0)
                  {
                    //dataSend.playercount++;
                    playercount++;
                    printf("Player %d connected! \n \t %x \n \t %d\n", i + 1, host.host, host.port);
                  }
                players[i] = host;
                playerNo = i;
                /*
                for (int i = 0; i < playercount; i++)
                  {
                    dataSend.player = message.player;
                    // Kolla så att det bara till skickas anslutna spelare och inte skickar tillbaka paketet till orginalsändaren
                    if (players[i].host != 0)
                      {
                        serverSendPacket(dataSend, &players[i], &sd);
                        printf ("sent data to player\n");
                      }
                  }
                  */
                break;
              }
          }
        //printf("we are here!");
        if (playerNo != -1) // Kollar ifall en spelar har anslutit sig/uppdaterat sin position
          {
            //we got a new message from playerNo
            printf("New packet received from Player %d at %x !\n\t %f %f %d \n", playerNo + 1, host.host, message.player.x, message.player.y, message.status);
            //struct PlayerPos position = {message.xPos, message.yPos};
            //dataSend.playerPositions[playerNo] = position;

            // Kollar ifall spelaren har disconnectat
            if (message.status == 3)
              {
                quit = 1;
              }

        // Skickar tillbaka uppdaterad data till alla anslutna spelare
        for (int i = 0; i < playercount; i++)
          {
            printf("\t%d\t",i);
            dataSend.player = message.player;
            printf("\thost: %d player[i].host: %d\n",host.host,players[i].host);
            if (players[i].host != 0)
            // Kolla så att det bara till skickas anslutna spelare och inte skickar tillbaka paketet till orginalsändaren
            if (players[i].host != 0 && !(players[i].host ==host.host && players[i].port == host.port)  )
              {
                serverSendPacket(dataSend, &players[i], &sd);
                printf ("sent data to player\n");
              }
          }
      }
    }

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta

    int frameTime = SDL_GetTicks() - frameStart; // Hur länge det tog att processa ramen

    if (frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }
  closeNetwork_Server(&sd, pSent);
  printf("server closed..\n");
  return 0;
}
