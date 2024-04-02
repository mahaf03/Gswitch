// GameView.c
#include "GameView.h"
#include <SDL2/SDL_image.h>

void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture) {
    *window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface = IMG_Load("resources/ship.png");
    *texture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);
}

void renderView(SDL_Renderer* renderer, SDL_Texture* texture, GameModel* model) {
    SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 }; // Adjust size as needed
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &shipRect);
    SDL_RenderPresent(renderer);
}

void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
