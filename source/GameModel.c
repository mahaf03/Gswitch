#include "GameModel.h"
#include "GameView.h"
#include <stdlib.h> // För rand() och srand()
#include <time.h>   // För time()

// void initializeModel(GameModel* model) {
//     model->x = 200;
//     model->y = 200;
//     model->velocityX = model->velocityY = 0;
//     model->up = model->down = model->left = model->right = false;
//     model->collisionUp = model->collisionDown = model->collisionLeft = model->collisionRight = false;
// }

void initializeModel(GameModel* model) {
   // Initialisera slumpgeneratorn
    srand((unsigned int)time(NULL));

    // Sätt spelarens initiala position och hastighet
    model->x = 200;
    model->y = 200;
    model->velocityX = model->velocityY = 0;

    // Initialisera spelarkontroller
    model->up = model->down = model->left = model->right = false;

    // Initialisera kollisionsstatusar
    model->collisionUp = model->collisionDown = model->collisionLeft = model->collisionRight = false;

    // Sätt hastigheten för blocken och initialisera deras positioner
    model->blockSpeed = 10; // Ange hastigheten för blocken
    for (int i = 0; i < 20; i++) {
        model->blockPositions[i].x = 1000 + i * 50; // Startar till höger utanför skärmen
        model->blockPositions[i].y = rand() % 750; // Ett slumpmässigt y-värde
        model->blockPositions[i].w = 50;
        model->blockPositions[i].h = 50;
    }
}

void updateBlocks(GameModel* model) {
    for (int i = 0; i < 20; i++) {
        // Uppdatera x-positionen för varje block
        model->blockPositions[i].x -= model->blockSpeed;

        // Kontrollera om blocket har passerat skärmens vänstra kant
        if (model->blockPositions[i].x < -model->blockPositions[i].w) {
            // Återställ blocket till höger utanför skärmen
            model->blockPositions[i].x = 1000;
            // Ge det en ny y-position
            model->blockPositions[i].y = rand() % (750 - model->blockPositions[i].h);
        }
    }
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int checkCollision(SDL_Rect shipRect, SDL_Rect blockRect) {
    if (shipRect.x + shipRect.w <= blockRect.x ||
        shipRect.x >= blockRect.x + blockRect.w ||
        shipRect.y + shipRect.h <= blockRect.y ||
        shipRect.y >= blockRect.y + blockRect.h) {
        return 0;
    }

    int overlapLeft = shipRect.x + shipRect.w - blockRect.x;
    int overlapRight = blockRect.x + blockRect.w - shipRect.x;
    int overlapTop = shipRect.y + shipRect.h - blockRect.y;
    int overlapBottom = blockRect.y + blockRect.h - shipRect.y;

    int minOverlap = min(min(overlapLeft, overlapRight), min(overlapTop, overlapBottom));

    if (minOverlap == overlapLeft) {
        return 1;
    } else if (minOverlap == overlapRight) {
        return 2;
    } else if (minOverlap == overlapTop) {
        return 3;
    } else if (minOverlap == overlapBottom) {
        return 4;
    }

    return 0;
}

void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks, SDL_Renderer* renderer, SDL_Texture* blockTexture) {
    bool anyCollision = false;
    
    for (int i = 0; i < numBlocks; i++) {
        int collisionResult = checkCollision(shipRect, blockPositions[i]);
        
        switch(collisionResult) {
            case 1: // Collision on the right side of the ship
                model->collisionLeft = true;
                model->x -= shipRect.x + shipRect.w - blockPositions[i].x;
                break;
            case 2: // Collision on the left side of the ship
                model->collisionRight = true;
                model->x += blockPositions[i].x + blockPositions[i].w - shipRect.x;
                break;
            case 3: // Collision on the top of the ship (ship is below)
                model->collisionDown = true;
                model->y -= shipRect.y + shipRect.h - blockPositions[i].y;
                break;
            case 4: // Collision on the bottom of the ship (ship is above)
                model->collisionUp = true;
                model->y += blockPositions[i].y + blockPositions[i].h - shipRect.y;
                break;
        }

        if(collisionResult != 0){
            anyCollision = true;
        }
        
    }

    if (!anyCollision) {
        model->collisionRight = false;
        model->collisionLeft = false;
        model->collisionUp = false;
        model->collisionDown = false;
    }
}
