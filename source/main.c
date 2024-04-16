//main.c
#include <SDL2/SDL.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"

int main(int argv, char** args) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;    // Skeppets textur
    SDL_Texture* bgTexture = NULL;  // Bakgrundstextur
    SDL_Texture* blockTexture = NULL; // Blockets textur

    GameModel model;
    initializeModel(&model);
    
    initView(&renderer, &window, &texture, &bgTexture, &blockTexture);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta

    Uint32 frameStart;
    int frameTime;

    bool closeWindow = false;
    while (!closeWindow) { // Game Loop
        frameStart = SDL_GetTicks(); // Tidpunkten då ramen startar

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
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

    closeView(renderer, window, texture, bgTexture, blockTexture);
    SDL_Quit();
    return 0;
}
