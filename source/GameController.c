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
                    if(!model->collisionUp){
                        model->up = true;
                        break;
                    }
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if(!model->collisionLeft){
                        model->left = true;
                        break;
                    }
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(!model->collisionDown){
                        model->down = true;
                        break;
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if(!model->collisionRight){
                        model->right = true;
                        break;
                    }
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
void applyGravity(GameModel* model) {
    const float gravity = 3.0f;  
   
    if (!model->collisionDown) {
        model->velocityY += gravity;
    }
}

void updateModel(GameModel* model) {
    //const float speed = 7.0f;
    model->velocityX = model->velocityY = 0;

    // applyGravity(model);

    if (model->up && !model->down && !model->collisionUp) model->velocityY = -model->playerSpeed;
    if (model->down && !model->up && !model->collisionDown) model->velocityY = model->playerSpeed;

    if (model->left && !model->right && !model->collisionLeft) model->velocityX = -model->playerSpeed;
    if (model->right && !model->left && !model->collisionRight) model->velocityX = model->playerSpeed; // Prevent right movement if collision on the right

    //if(check(model(x+1)))
    model->x += model->velocityX;
    model->y += model->velocityY;

}

void stopModel(GameModel* model, int direction) {
    if(direction == 1){
        model->velocityX = 0;
        model->left = model->right = false;
        model->collisionRight = true;
    }
    if(direction == 2){
        model->velocityX = 0;
        model->left = model->right = false;
        model->collisionLeft = true;
    }
    if(direction == 3){
        model->velocityY = 0;
        model->up = model->down = false;
        model->collisionUp = true;
    }
    if(direction == 4){
        model->velocityY = 0;
        model->up = model->down = false;
        model->collisionDown = true;
    }
    if(direction == 0){
        model->collisionRight = false;
        model->collisionLeft = false;
        model->collisionUp = false;
        model->collisionDown = false;
    }

}
