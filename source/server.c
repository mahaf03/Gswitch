#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_timer.h"
#include "Network.h"
typedef struct  sender{
  udpDataToClient* data;
  IPaddress* player;
  UDPsocket * sd;
} sender;
Uint32 callback ( Uint32 interval, void* name)
{
  sender Data =  *(sender* )name;
  serverSendPacket (*Data.data,Data.player,Data.sd);
  return interval;
}


int main(int argc,char** argv)
  {
    SDL_Init(SDL_INIT_TIMER);
    UDPsocket sd;
    UDPpacket* pSent;
    printf("opening server..\n");
    udpDataToClient dataSend= {};
    dataSend.status = 0;
    dataSend.playercount = 0;
    IPaddress players[4];
            for (int i = 0;i<4;i++)
              {
                IPaddress empty = {0,0};
                players[i]= empty;
              }
    initNetwork_Server (&sd);
    if (!((pSent = SDLNet_AllocPacket(512))))
      {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
      }
    sender sendy;
    //sendy.data = &dataSend;
    //sendy.players = &players[0];
    //sendy.sd = &sd;
    //SDL_TimerID  timerID = SDL_AddTimer(10,callback,&sendy);
    udpDataToServer message;
    int quit = 0;
    while (quit==0)
      {
       IPaddress host;
       host = serverReceivePacket(&message,&sd);
        if( host.host != 0 && host.port !=0)
          {
            int playerNo = 0;
            for (int i = 0;i<4;i++)
              {
                if ((players[i].host  == 0 && players[i].port ==0 ) || (host.host== players [i].host&& host.port == players[i].port))
                  {
                    if(players[i].host  == 0)
                      {
                        dataSend.playercount++;
                        printf("Player %d connected! \n \t %x \n \t %d\n", i+1, host.host,host.port);
                        sender sendy;
                        sendy.data = &dataSend;
                        sendy.player = &players[i];
                        sendy.sd = &sd;
                        SDL_TimerID  timerID = SDL_AddTimer(100/60,callback,&sendy);
                      }
                    players[i] = host;
                    playerNo = i+1;
                    break;
                  }
              }
            printf("New packet received from Player %d at %x !\n\t %f %f %d \n",playerNo,host.host,message.xPos,message.yPos,message.status);
            struct PlayerPos position = {message.xPos,message.yPos};
            dataSend.playerPositions[playerNo-1] = position;
            if (message.status == 3)
              {quit = 1;}
            //printf ("playercount: %d \n", dataSend.playercount);
            for (int i = 0; i < dataSend.playercount; i++)
              {
                //printf ("Sending!\n\t player %d\t %f, %f\n",i+1,dataSend.playerPositions[i].x,dataSend.playerPositions[i].y);
                //serverSendPacket (dataSend,&players[i],&sd);
                //printf("sent\n");
              }

          }
                 SDL_Delay(10); // Approximate 60 FPS
      }
    closeNetwork_Server(&sd,pSent);
    printf("server closed..\n");
    return 0;
  }
