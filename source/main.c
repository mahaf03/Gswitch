
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
    initNetwork_Client(&sd,&srvadd,p,pReceive);
    printf("Network initialized!\n");

    bool closeWindow = false;
    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
            struct udpData testdata ={1,3,1};
            printf("sending data \n\t %d %d %d\n",testdata.xPos,testdata.yPos,testdata.status);
            //sprintf((char*)p->data,"%d %d %d\n", 1,2,3);

            sendPacket(p,&srvadd,&sd);
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
