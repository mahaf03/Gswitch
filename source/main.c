
#include <SDL2/SDL.h>
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

    GameModel model;
    initializeModel(&model);
    
    // Du måste också passera en pekare till bakgrundstexturen till initView
    initView(&renderer, &window, &texture, &bgTexture);
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
    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
            struct udpData testdata ={1,1,0};
            printf("sending data \n\t %d %d %d\n",testdata.xPos,testdata.yPos,testdata.status);
            //send a message to server, if testdata.status == 3 then the server shuts down.
            if (model.x >= 400.f)
                { testdata.status =3;}
            sendPacket(testdata,&srvadd,&sd);
            printf("data sent!\n");
        }

        updateModel(&model);
        
        // Uppdatera renderView-anropet för att inkludera bakgrundstexturen
        renderView(renderer, texture, bgTexture, &model);
        
        SDL_Delay(1000 / 60); // Approximate 60 FPS
    }

    // Glöm inte att skicka bgTexture till closeView så att det också kan rensas upp
    closeView(renderer, window, texture, bgTexture);
    SDL_Quit();
    return 0;
}
