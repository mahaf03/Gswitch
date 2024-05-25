#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "Network.h"
#include "GameModel.h"

void sendWinnerDataToClients(int winnerId, UDPsocket sd, IPaddress *players, int playerCount)
{
    udpDataToClient winnerData;
    winnerData.clientId = winnerId;
    winnerData.gameReady = false; // Indikera att spelet är slut

    for (int i = 0; i < playerCount; i++)
    {
        if (players[i].host != 0)
        {
            serverSendPacket(winnerData, &players[i], &sd);
        }
    }
}

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
                if (message.player.isDead)
                {
                    gameModel.playercount--; //testa senare
                }
                memcpy(&dataSend.next30Rand, &next30Rand, sizeof(int) * 30);
                // resend the message to all other connected clients
                for (int i = 0; i < 4; i++)
                {
                    if (players[i].host != 0)
                    {
                        dataSend.clientId = i;
                        serverSendPacket(dataSend, &players[i], &sd);
                        printf("sent data to player %d\n", i + 1);
                    }
                }

                // Kontrollera vinnare
                int aliveCount = 0;
                int lastAlivePlayerId = -1;
                for (int i = 0; i < 4; i++)
                {
                    if (!message.player.isDead)
                    {
                        aliveCount++;
                        lastAlivePlayerId = i;
                    }
                }
                if (aliveCount == 1)
                {
                    sendWinnerDataToClients(lastAlivePlayerId, sd, players, 4);
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

