#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "Network.h"
#include "GameModel.h"
#define MAX_PLAYERS 4

// void resetGameServer(UDPsocket *sd, IPaddress *players, GameModel *gameModel, int *next30Rand)
// {
//   // Stänga eventuella öppna nätverkssessioner och återöppna dem
//   if (*sd != NULL)
//   {
//     SDLNet_UDP_Close(*sd);
//     *sd = SDLNet_UDP_Open(49156);
//     if (*sd == NULL)
//     {
//       fprintf(stderr, "Failed to reopen UDP socket: %s\n", SDLNet_GetError());
//       exit(EXIT_FAILURE);
//     }
//   }

//   // Återställa spelarantal och adresser
//   gameModel->playercount = 0;
//   for (int i = 0; i < 4; i++)
//   {
//     players[i].host = 0;
//     players[i].port = 0;
//   }

//   // Återställa spelmodellens data
//   memset(gameModel, 0, sizeof(GameModel));
//   initializeModel(gameModel); // Förutsätter att denna funktion återställer GameModel helt

//   // Återställa slumpmässiga tal för blockpositioner eller annan relevant data
//   for (int i = 0; i < 30; i++)
//   {
//     next30Rand[i] = rand() % 700; // Eller vilken beräkningslogik du har för dessa
//   }

//   printf("Server has been reset. All players disconnected and server is ready for new players.\n");
// }

int main(int argc, char **argv)
{

  UDPsocket sd;
  UDPpacket *pSent;
  Uint32 startTime;
  Uint32 startCount;
  printf("opening server..\n");
  udpDataToClient dataSend = {};
  dataSend.status = 0;
  IPaddress players[4];
  bool playerDead[4];
  int next30Rand[30];
  GameModel gameModel;
  gameModel.playercount = 0;
  for (int i = 0; i < 30; i++)
  {
    next30Rand[i] = rand() % 700;
  }
  // initialize array of players Ipaddresses to be empty.
  for (int i = 0; i < 4; i++)
  {
    IPaddress empty = {0, 0};
    players[i] = empty;
    playerDead[i] = false;
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
      // if (message.status == STATUS_RESET_PLAYERCOUNT)
      // {
      //   resetGameServer(&sd, players, &gameModel, next30Rand);
      // }
      int playerNo = -1;
      for (int i = 0; i < 4; i++)
      {
        if (players[i].host == 0 && players[i].port == 0)
        {
          gameModel.playercount++;
          if (gameModel.playercount == 4)
          {
            dataSend.gameReady = true;
            printf("Fyra spelare anslutna, spelet startar!\n");
            startCount = startTime = SDL_GetTicks();
          }
          printf("Player %d connected! \n \t %x \n \t %d\n", i + 1, host.host, host.port);
          players[i] = host;
          playerNo = i;
          break;
        }
        else if (host.host == players[i].host && host.port == players[i].port)
        {
          // the message is from already connected player
          playerNo = i;
          playerDead[i] = message.player.isDead;
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

        // if (message.player.isDead)
        // {
        //   gameModel.playercount--; // testa senare
        // }
        memcpy(&dataSend.next30Rand, &next30Rand, sizeof(int) * 30);
        // resend the message to all other connected clients
        for (int i = 0; i < gameModel.playercount; i++)
        {
          printf("\t%d\t", i);
          printf("\thost: %d player[i].host: %d\n", host.host, players[i].host);

          if (players[i].host != 0)
          {
            dataSend.clientId = i;
            serverSendPacket(dataSend, &players[i], &sd);
            printf("sent data to player\n");
          }
        }

        int deadCount = 0;
        for (int i = 0; i < gameModel.playercount; i++)
        {
          if (playerDead[i] == true)
          {
            deadCount++;
          }
        }
        printf("deadcount %d ", deadCount);

        if (deadCount == gameModel.playercount)
        {
          for (int i = 0; i < gameModel.playercount; i++)
          {
            playerDead[i] = false;
            players[i].host = 0;
            players[i].port = 0;
          }

          gameModel.playercount = 0;
          dataSend.gameReady = false;
          printf("Vi nollställer \n");
        }
      }
    }
    if (SDL_GetTicks() - startCount > 5000)
    {
      startCount = SDL_GetTicks();
      for (int i = 0; i < 30; i++)
      {
        next30Rand[i] = rand() % 700;
      }
    }
  }
  closeNetwork_Server(&sd, pSent);
  printf("server closed..\n");
  return 0;
}
