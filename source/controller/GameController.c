// GameController.c
#include "GameController.h"

void handleEvent(SDL_Event* event, GameModel* model, bool* closeWindow) {
    switch (event->type) {
        case SDL_QUIT:
            *closeWindow = true;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    model->up = true;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    model->left = true;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    model->down = true;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    model->right = true;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    model->up = false;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    model->left = false;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    model->down = false;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    model->right = false;
                    break;
            }
            break;
    }
}

void updateModel(GameModel* model) {
    const float speed = 0.1f;
    model->velocityX = model->velocityY = 0;
    if (model->up && !model->down) model->velocityY = -speed;
    if (model->down && !model->up) model->velocityY = speed;
    if (model->left && !model->right) model->velocityX = -speed;
    if (model->right && !model->left) model->velocityX = speed;

    model->x += model->velocityX;
    model->y += model->velocityY;
    
    // Keep the ship within window bounds
    if (model->x < 0) model->x = 0;
    if (model->y < 0) model->y = 0;
    if (model->x > 600 - 50) model->x = 600 - 50; // Window width - ship width
    if (model->y > 400 - 50) model->y = 400 - 50; // Window height - ship height
}
