#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "Network.h"

int main(int argc, char **argv)
{
  UDPsocket sd;
  UDPpacket *pSent;
  Uint32 frameStart;
  printf("opening server..\n");
  udpDataToClient dataSend = {};
  dataSend.status = 0;
  IPaddress players[4];
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
    if (host.host != 0 && host.port != 0)
    {
      int playerNo = -1;
      for (int i = 0; i < 4; i++)
      {
        if ((players[i].host == 0 && players[i].port == 0) || (host.host == players[i].host && host.port == players[i].port))
        {
          if (players[i].host == 0 && players[i].port == 0)
          {
            dataSend.playercount++;
            printf("Player %d connected! \n \t %x \n \t %d\n", i + 1, host.host, host.port);
          }
          players[i] = host;
          playerNo = i;
          break;
        }
      }

      if (playerNo != -1) // Kollar ifall en spelar har anslutit sig/uppdaterat sin position
      {
        printf("New packet received from Player %d at %x !\n\t %f %f %d \n", playerNo + 1, host.host, message.xPos, message.yPos, message.status);
        struct PlayerPos position = {message.xPos, message.yPos};
        dataSend.playerPositions[playerNo] = position;

        // Kollar ifall spelaren har disconnectat
        if (message.status == 3)
        {
          quit = 1;
        }

        // Skickar tillbaka uppdaterad data till alla anslutna spelare
        for (int i = 0; i < dataSend.playercount; i++)
        {
          if (players[i].host != 0) // Kollar så att det bara skickas till anslutna spelare
          {
            serverSendPacket(dataSend, &players[i], &sd);
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