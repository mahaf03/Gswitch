// GameView.c
#include "GameView.h"
#include <SDL2/SDL_image.h>

// void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture) {
//     *window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
//     *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
//     SDL_Surface* surface = IMG_Load("resources/ship.png");
//     *texture = SDL_CreateTextureFromSurface(*renderer, surface);
//     SDL_FreeSurface(surface);
// }

// void renderView(SDL_Renderer* renderer, SDL_Texture* texture, GameModel* model) {
//     SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 }; // Adjust size as needed
//     SDL_RenderClear(renderer);
//     SDL_RenderCopy(renderer, texture, NULL, &shipRect);
//     SDL_RenderPresent(renderer);
// }

// void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture) {
//     SDL_DestroyTexture(texture);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
// }

// GameView.c

// ... din tidigare kod ...

void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture, SDL_Texture** backgroundTexture ,SDL_Texture** blockTexture) {
    *window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface = IMG_Load("resources/austrounat.png");
    *texture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    // Ladda bakgrundstexturen
    loadBackground(*renderer, backgroundTexture);
    loadBlock(*renderer, blockTexture);
}

void loadBackground(SDL_Renderer* renderer, SDL_Texture** backgroundTexture) {
    SDL_Surface* surface = IMG_Load("resources/background.png");
    if (!surface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        *backgroundTexture = NULL;
        return;
    }
    *backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void loadBlock(SDL_Renderer* renderer, SDL_Texture** blockTexture) {
    SDL_Surface* surface = IMG_Load("resources/speedTile3.png");
    if (!surface) {
        printf("Unable to load block image: %s\n", IMG_GetError());
        *blockTexture = NULL;
        return;
    }
    *blockTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void renderView(SDL_Renderer* renderer, SDL_Texture* shipTexture, SDL_Texture* backgroundTexture, SDL_Texture* blockTexture, GameModel* model) {
    SDL_RenderClear(renderer);

    // Först, rendera bakgrundstexturen
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // Ritar ut över hela skärmen
    }

    // Sedan, rendera skeppet
    SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 }; // Justera storlek som behövs
    SDL_RenderCopy(renderer, shipTexture, NULL, &shipRect);
    
    // sedan blocken
    SDL_Rect blockRect = { 0, 0, 50, 50 };
    SDL_RenderCopy(renderer, blockTexture, NULL, &blockRect);


    SDL_RenderPresent(renderer);
}

void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* shipTexture, SDL_Texture* backgroundTexture,SDL_Texture* blockTexture) {
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyTexture(backgroundTexture); // Rensa bakgrundstexturen
    SDL_DestroyTexture(blockTexture); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

