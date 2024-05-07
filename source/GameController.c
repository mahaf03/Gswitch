// GameController.c
#include "GameController.h"

void handleEvent(SDL_Event* event, Player* player, bool* closeWindow) {
    switch (event->type) {
        case SDL_QUIT:
            *closeWindow = true;
            break;

        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if(!player->collisionUp){
                          player->up = true;
                    }
                    break;
                    
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if(!player->collisionLeft){
                        player->left = true;
                    }
                    break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                    if(!player->collisionDown){
                        player->down = true;
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if(!player->collisionRight){
                        player->right = true;
                    }
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    player->up = false;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    player->left = false;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    player->down = false;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    player->right = false;
                    break;
            }
            break;
    }
}
// void applyGravity(Player* player) {
//     const float gravity = 1.0f;  
//     for(int i = 0; i < 4; i++){
//         if (!player->collisionDown) {
//             player->velocityY += gravity;
//         }
//     }
// }

void updatePlayer(Player* player) {
    //const float speed = 7.0f;
    
    player->velocityX = player->velocityY = 0;

    //  applyGravity(player);

    if (player->up && !player->down && !player->collisionUp) player->velocityY = -player->playerSpeed;
    if (player->down && !player->up && !player->collisionDown) player->velocityY = player->playerSpeed;

    if (player->left && !player->right && !player->collisionLeft) player->velocityX = -player->playerSpeed;
    if (player->right && !player->left && !player->collisionRight) player->velocityX = player->playerSpeed; // Prevent right movement if collision on the right

    //if(check(model(x+1)))
    player->x += player->velocityX;
    player->y += player->velocityY;

}

void stopPlayer(Player* player, int direction) {
    if(direction == 1){
        player->velocityX = 0;
        player->left = player->right = false;
        player->collisionRight = true;
    }
    if(direction == 2){
        player->velocityX = 0;
        player->left = player->right = false;
        player->collisionLeft = true;
    }
    if(direction == 3){
        player->velocityY = 0;
        player->up = player->down = false;
        player->collisionUp = true;
    }
    if(direction == 4){
        player->velocityY = 0;
        player->up = player->down = false;
        player->collisionDown = true;
    }
    if(direction == 0){
        player->collisionRight = false;
        player->collisionLeft = false;
        player->collisionUp = false;
        player->collisionDown = false;
    }

}
