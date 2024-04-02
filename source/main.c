
// main.c
#include <SDL2/SDL.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"

int main(int argv, char** args) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    GameModel model;
    initializeModel(&model);
    initView(&renderer, &window, &texture);

    bool closeWindow = false;
    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &model, &closeWindow);
        }

        updateModel(&model);
        renderView(renderer, texture, &model);
        SDL_Delay(1000 / 60); // Approximate 60 FPS
    }


    closeView(renderer, window, texture);
    SDL_Quit();
    return 0;
}
