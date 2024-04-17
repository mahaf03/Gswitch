//main.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "Network.h"

int main(int argv, char** args) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;    // Skeppets textur
    SDL_Texture* bgTexture = NULL;  // Bakgrundstextur
    SDL_Texture* blockTexture = NULL; // Blockets textur

    GameModel model;
    initializeModel(&model);

    bool success = true;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    Mix_Music *backgroundMusic = Mix_LoadMUS("resources/music.mp3");
    if (backgroundMusic == NULL) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Spela musik
    Mix_PlayMusic(backgroundMusic, -1); // Sista argumentet är antalet repetitioner (-1 för oändlig loop)

    initView(&renderer, &window, &texture, &bgTexture, &blockTexture);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta

    Uint32 frameStart;
    int frameTime;
    GameModel models[4];
    int playercount = 0;
    for (int i=0;i<4;i++)
        {
            initializeModel(&models[i]);
        }
    // Du måste också passera en pekare till bakgrundstexturen till initView
    // initView(&renderer, &window, &texture, &bgTexture);
    printf("initializing network... \n");
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket* pReceive;
    UDPpacket* p;
    initNetwork_Client(&sd,&srvadd,pReceive);
    printf("Network initialized!\n");
    //Allocate memory for UDP packets
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

    bool closeWindow = false;
    while (!closeWindow) { // Game Loop
        frameStart = SDL_GetTicks(); // Tidpunkten då ramen startar

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
            udpDataToServer testdata ={model.x,model.y,0};
            //printf("sending data \n\t %f %f %d\n",testdata.xPos,testdata.yPos,testdata.status);
            //send a message to server, if testdata.status == 3 then the server shuts down.
            if (model.x >= 400.f)
                { testdata.status =3;}
            clientSendPacket(testdata,&srvadd,&sd);

            //printf("data sent!\n");
        }
        udpDataToClient message;
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
        for (int i = 0; i<playercount;i++)
            {
                models[i].x=message.playerPositions[i].x;
                models[i].y=message.playerPositions[i].y;
                updateModel(&models[i]);
            }

        //Skapa ship
        // SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 };
        SDL_Rect shipRect = { (int)model.x, (int)model.y, 50, 50 };


        updateModel(&model);
        updateBlocks(&model,shipRect); // Lägg till denna rad för att uppdatera blocken

        renderView(renderer, texture, bgTexture, blockTexture, &model,shipRect);

        frameTime = SDL_GetTicks() - frameStart; // Hur länge det tog att processa ramen

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    Mix_FreeMusic(backgroundMusic); // För musik
    backgroundMusic = NULL;

    Mix_CloseAudio(); // Vid avslutning av programmet

    closeView(renderer, window, texture, bgTexture, blockTexture);
    SDL_Quit();
    return 0;
}
