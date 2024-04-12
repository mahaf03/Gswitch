// GameView.c
#include "GameView.h"
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "GameController.h"

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

// int checkCollision(SDL_Rect shipRect, SDL_Rect blockRect) {
    
//     if((shipRect.x + shipRect.w > blockRect.x && shipRect.y + shipRect.h > blockRect.y) ||(shipRect.x + shipRect.w > blockRect.x && shipRect.y < blockRect.y + blockRect.h)){
//         printf("1\n");
//         return 1; //kollision höger
//     } 
//     else if(shipRect.x < blockRect.x + blockRect.w){
//         printf("2\n");
//         return 2; //kollision vänster
//     }
//     else if(shipRect.y + shipRect.h > blockRect.y){
//         printf("3\n");
//         return 3; //kollision uppifrån
//     }
//     else if(shipRect.y < blockRect.y + blockRect.h){
//         printf("4\n");
//         return 4; //kollision underifrån
//     }
//     else{
//         printf("0\n");
//         return 0;
//     }
// }
int checkCollision(SDL_Rect shipRect, SDL_Rect blockRect) {
    // Check for no collision first
    if (shipRect.x + shipRect.w <= blockRect.x ||  // ship is to the left of block
        shipRect.x >= blockRect.x + blockRect.w || // ship is to the right of block
        shipRect.y + shipRect.h <= blockRect.y ||  // ship is above block
        shipRect.y >= blockRect.y + blockRect.h) { // ship is below block
        return 0; // No collision
    }

    // Determine the side of the collision
    int shipRight = shipRect.x + shipRect.w;
    int shipBottom = shipRect.y + shipRect.h;
    int blockRight = blockRect.x + blockRect.w;
    int blockBottom = blockRect.y + blockRect.h;

    // Horizontal collision detection
    int shipMidX = shipRect.x + shipRect.w / 2;
    int blockMidX = blockRect.x + blockRect.w / 2;
    int shipMidY = shipRect.y + shipRect.h / 2;
    int blockMidY = blockRect.y + blockRect.h / 2;

    if (shipMidX < blockMidX && shipRight > blockRect.x) {
        printf("Collision on Right\n");
        return 1; // Collision on the right side of the ship
    }
    if (shipMidX > blockMidX && shipRect.x < blockRight) {
        printf("Collision on Left\n");
        return 2; // Collision on the left side of the ship
    }
    if (shipMidY < blockMidY && shipBottom > blockRect.y) {
        printf("Collision on Top\n");
        return 3; // Collision on the top of the ship (ship is below)
    }
    if (shipMidY > blockMidY && shipRect.y < blockBottom) {
        printf("Collision on Bottom\n");
        return 4; // Collision on the bottom of the ship (ship is above)
    }

    return 0; // Fallback, should not be reached
}



void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks, SDL_Renderer* renderer, SDL_Texture* blockTexture) {
    bool anyCollision = false;
    
    for (int i = 0; i < numBlocks; i++) {
        int collisionResult = checkCollision(shipRect, blockPositions[i]);
        stopModel(model, collisionResult);
        if(collisionResult != 0){
            anyCollision = true;
        }
    }

    if (!anyCollision) {
        model->collisionRight = false; // Reset collision state if no collisions detected
        model->collisionLeft = false;
        model->collisionUp = false;
        model->collisionDown = false;
    }
}


void renderView(SDL_Renderer* renderer, SDL_Texture* shipTexture, SDL_Texture* backgroundTexture, SDL_Texture* blockTexture, GameModel* model) {
    SDL_RenderClear(renderer);

    // Antag att dessa är dina blockpositioner
    // SDL_Rect blockPositions[] = {
    //     {0, 0, 50, 50}, {50, 0, 50, 50}, // Övre vänstra hörnet
    //     {0, 750, 50, 50}, {50, 750, 50, 50}, {100, 750, 50, 50}, // Nedre vänstra hörnet
    //     {150, 750, 50, 50}, {200, 750, 50, 50}, {250, 750, 50, 50}, // Fler positioner...
    //     {500, 750, 50, 50}, {550, 750, 50, 50}, {600, 750, 50, 50},
    //     {650, 750, 50, 50}, {700, 750, 50, 50},
    //     {600, 400, 50, 50}, {650, 400, 50, 50}, {700, 400, 50, 50}, {750, 400, 50, 50}
    // };
        SDL_Rect blockPositions[] = {
        {600, 400, 50, 50}
    };
    int numBlocks = sizeof(blockPositions) / sizeof(blockPositions[0]);

    // Rendera bakgrunden
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Rendera skeppet
    SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 };
    SDL_RenderCopy(renderer, shipTexture, NULL, &shipRect);


   handleCollision(model, shipRect, blockPositions, numBlocks, renderer, blockTexture);


    // Rendera blocken från listan
    for (int i = 0; i < numBlocks; i++) {
        placeTile(renderer, blockTexture, blockPositions[i].x, blockPositions[i].y);
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


