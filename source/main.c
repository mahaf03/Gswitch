
#include <SDL2/SDL.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "Network.h"
typedef struct receiver {
    UDPsocket* sd;
    int* playercount;
    GameModel* models;
}receiver;
Uint32 callback ( Uint32 interval, void* name)
    {
        receiver rec = *(receiver*)name;
        udpDataToClient message;
        IPaddress addrr = clientReceivePacket(&message,rec.sd);
        //printf("done!\n");
        if(addrr.host !=0 && addrr.port !=0  )
            {
                printf("got package\n");
                for(int i = 0; i< message.playercount; i++)
                    {
                        printf("\tplayer %d at %f %f\n",i,message.playerPositions[i].x,message.playerPositions[i].y);
                        *rec.playercount= message.playercount;
                        rec.models[i].x = message.playerPositions[i].x;
                        rec.models[i].y = message.playerPositions[i].y;
                    }
            }


        return interval;

    }
int main(int argv, char** args) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;    // Skeppets textur
    SDL_Texture* bgTexture = NULL;  // Bakgrundstextur

    GameModel model;
    initializeModel(&model);
    GameModel models[4];
    int playercount = 0;
    for (int i=0;i<4;i++)
        {
            initializeModel(&models[i]);
        }
    // Du måste också passera en pekare till bakgrundstexturen till initView
    initView(&renderer, &window, &texture, &bgTexture);
    printf("initializing network... \n");
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket* pReceive;
    UDPpacket* p;
    initNetwork_Client(&sd,&srvadd);
    printf("Network initialized!\n");
    receiver reccy;
    reccy.models = models;
    reccy.playercount = &playercount;
    reccy.sd = &sd;
    SDL_TimerID  timerID = SDL_AddTimer(1,callback,&reccy);
    //Allocate memory for UDP packets
/*
    if (!(p = SDLNet_AllocPacket(512)))
        {
            fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    if (!((pReceive= SDLNet_AllocPacket(512))))
        {
            fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
        */

    bool closeWindow = false;
    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
        }
        udpDataToServer testdata ={model.x,model.y,0};
        if (model.x >= 400.f)
            { testdata.status =3;}
        clientSendPacket(testdata,&srvadd,&sd);

        /*
        udpDataToClient message;
        IPaddress addrr = clientReceivePacket(&message,&sd);
        if(addrr.host !=0 && addrr.port !=0  )
            {
                printf("got package\n");
                for(int i = 0; i< message.playercount; i++)
                    {
                        printf("\tplayer %d at %f %f\n",i,message.playerPositions[i].x,message.playerPositions[i].y);
                        playercount = message.playercount;
                        models[i].x = message.playerPositions[i].x;
                        models[i].y = message.playerPositions[i].y;
                    }
            }
*/
        /*udpDataToClient message;
        //IPaddress addrr = clientReceivePacket(&message,&sd);
        IPaddress addrr = clientReceivePacket(&message,&sd);
        if(addrr.host !=0 && addrr.port !=0  )
            {
                printf( "new message from %x:\n",addrr.host);
                playercount = message.playercount;
                for(int i = 0; i< message.playercount; i++)
                    {
                        printf("\tplayer %d at %f %f\n",i,message.playerPositions[i].x,message.playerPositions[i].y);
                        models[i].x = message.playerPositions[i].x;
                        models[i].y = message.playerPositions[i].y;
                    }
            }
*/
        for (int i = 0; i<playercount;i++)
            {
                updateModel(&models[i]);
            }
        updateModel(&model);
        
        // Uppdatera renderView-anropet för att inkludera bakgrundstexturen
        //printf("playercount: %d \n",playercount);
        renderView(renderer, texture, bgTexture, &models[0],playercount);
        if(playercount == 0)
            renderView(renderer, texture, bgTexture, &model,1);
        SDL_Delay(1000 / 60); // Approximate 60 FPS
    }

    // Glöm inte att skicka bgTexture till closeView så att det också kan rensas upp
    closeView(renderer, window, texture, bgTexture);
    closeNetwork_Client(&sd);
    SDL_Quit();
    return 0;
}
