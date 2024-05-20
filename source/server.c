#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "Network.h"
// #define MAX_PLAYERS 4;

int main(int argc, char **argv)
{

  UDPsocket sd;
  UDPpacket *pSent;
  Uint32 frameStart;
  printf("opening server..\n");
  udpDataToClient dataSend = {};
  int playercount = 0;
  dataSend.status = 0;
  IPaddress players[4];

  // initialize array of players Ipaddresses to be empty.
  for (int i = 0; i < 4; i++)
  {
    IPaddress empty = {0, 0};
    players[i] = empty;
  }
  initNetwork_Server(&sd);
  udpDataToServer message;
  int quit = 0;
  while (quit == 0)
  {
    IPaddress host;
    // receive the packet. host contains the IPaddress and port of the client who sent it.
    host = serverReceivePacket(&message, &sd);
    int gotPkg = (host.host != 0 && host.port != 0);
    if (gotPkg) // did we get a packet?
    {
      int playerNo = -1;
      for (int i = 0; i < 4; i++)
      {
        if (players[i].host == 0 && players[i].port == 0)
        {
          // the message is from a not already connected player
          playercount++;
          if (playercount == 2)
          {
            dataSend.gameReady = true;
            printf("Fyra spelare anslutna, spelet startar!\n");
          }
          printf("Player %d connected! \n \t %x \n \t %d\n", i + 1, host.host, host.port);
          players[i] = host;
          playerNo = i;
          break;

          // if (playercount == 4)
          // {
          //   fourPlayers = true;
          // }
          // // här ökar vi antalet spelare
          // // hur gör vi så att spelet börjar när vi har 4 spelare?
          // printf("Player %d connected! \n \t %x \n \t %d\n", i + 1, host.host, host.port);
          // players[i] = host;
          // playerNo = i;
          // break;
        }
        else if (host.host == players[i].host && host.port == players[i].port)
        {
          // the message is from already connected player
          playerNo = i;
          break;
        }
      }
      if (playerNo != -1) // Kollar ifall en spelar har anslutit sig/uppdaterat sin position
      {
        // we got a new message from player number: playerNo
        printf("New packet received from Player %d at %x !\n\t %f %f %d \n", playerNo + 1, host.host, message.player.x, message.player.y, message.status);

        // Kollar ifall spelaren har disconnectat
        if (message.status == 1)
        {
          quit = 1;
        }

        dataSend.player = message.player;
        // resend the message to all other connected clients
        for (int i = 0; i < playercount; i++)
        {
          printf("\t%d\t", i);
          printf("\thost: %d player[i].host: %d\n", host.host, players[i].host);

          // resend the packet to al players.
          if (players[i].host != 0)
          {
            dataSend.clientId = i;
            serverSendPacket(dataSend, &players[i], &sd);
            printf("sent data to player\n");
          }
        }
      }
    }
  }
  closeNetwork_Server(&sd, pSent);
  printf("server closed..\n");
  return 0;
}
