// GameView.c
#include <SDL2/SDL_image.h>
#include "GameView.h"
#include <stdbool.h>
#include "GameController.h"
#include "GameModel.h"

void drawLives(SDL_Renderer* renderer, int lives, int x, int y) {
    int barWidth = 15;
    SDL_Color color = {0, 255, 255}; // blå färg för livsindikatorn
    for (int i = 0; i < lives; i++) {
        SDL_Rect rect = {x + i * (barWidth + 5), y, barWidth, 20};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture, SDL_Texture** backgroundTexture ,SDL_Texture** blockTexture) {
    *window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 686, 0);
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




void renderView(SDL_Renderer* renderer, SDL_Texture* shipTexture, SDL_Texture* backgroundTexture, SDL_Texture* blockTexture, GameModel* model, SDL_Rect shipRect) {
    SDL_RenderClear(renderer);
    

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }


    SDL_RenderCopy(renderer, shipTexture, NULL, &shipRect);


    for (int i = 0; i < 20; i++) {
        placeTile(renderer, blockTexture, model->blockPositions[i].x, model->blockPositions[i].y);
        drawLives(renderer, model->playerLife, 10, 10); // Visa rektanglar som liv nedanför texten
    }
    


    SDL_RenderPresent(renderer);
}


void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* shipTexture, SDL_Texture* backgroundTexture,SDL_Texture* blockTexture) {
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyTexture(backgroundTexture); // Rensa bakgrundstexturen
    SDL_DestroyTexture(blockTexture); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void placeTile(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
    SDL_Rect tileRect = { x, y, 50, 50 }; // Antag att varje tile är 50x50 pixlar
    SDL_RenderCopy(renderer, texture, NULL, &tileRect);
}




