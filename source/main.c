
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
    
    // Du måste också passera en pekare till bakgrundstexturen till initView
    initView(&renderer, &window, &texture, &bgTexture, &blockTexture);

    bool closeWindow = false;
    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
        }

        updateModel(&model);
        
        // Uppdatera renderView-anropet för att inkludera bakgrundstexturen
        renderView(renderer, texture, bgTexture, blockTexture, &model);

        
        // SDL_Delay(1000 / 60); // Approximate 60 FPS
    }

    // Glöm inte att skicka bgTexture till closeView så att det också kan rensas upp
    closeView(renderer, window, texture, bgTexture, blockTexture);
    SDL_Quit();
    return 0;
}
