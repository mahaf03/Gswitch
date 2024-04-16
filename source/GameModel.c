// GameModel.c
#include "GameModel.h"
#include "GameView.h"

void initializeModel(GameModel* model) {
    model->x = 200; // Half of WINDOW_WIDTH
    model->y = 200; // Half of WINDOW_HEIGHT
    model->velocityX = model->velocityY = 0;
    model->up = model->down = model->left = model->right = false;
    model->collisionUp = model->collisionDown = model->collisionLeft = model->collisionRight = false;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int checkCollision(SDL_Rect shipRect, SDL_Rect blockRect) {
    // Check for no collision first
    if (shipRect.x + shipRect.w <= blockRect.x ||  // Ship is to the left of block
        shipRect.x >= blockRect.x + blockRect.w || // Ship is to the right of block
        shipRect.y + shipRect.h <= blockRect.y ||  // Ship is above block
        shipRect.y >= blockRect.y + blockRect.h) { // Ship is below block
        return 0; // No collision
    }

    // Calculate overlaps on each side
    int overlapLeft = shipRect.x + shipRect.w - blockRect.x;
    int overlapRight = blockRect.x + blockRect.w - shipRect.x;
    int overlapTop = shipRect.y + shipRect.h - blockRect.y;
    int overlapBottom = blockRect.y + blockRect.h - shipRect.y;

    // Find the least overlap to determine the collision side
    int minOverlap = min(min(overlapLeft, overlapRight), min(overlapTop, overlapBottom));

    if (minOverlap == overlapLeft) {
        return 1; // Collision on the right side of the ship
    } else if (minOverlap == overlapRight) {
        return 2; // Collision on the left side of the ship
    } else if (minOverlap == overlapTop) {
        return 3; // Collision on the top of the ship (ship is below)
    } else if (minOverlap == overlapBottom) {
        return 4; // Collision on the bottom of the ship (ship is above)
    }

    return 0; // Fallback, should not be reached
}

void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks, SDL_Renderer* renderer, SDL_Texture* blockTexture) {
    bool anyCollision = false;
    
    for (int i = 0; i < numBlocks; i++) {
        int collisionResult = checkCollision(shipRect, blockPositions[i]);
        // printf("Collision result: %d\n", collisionResult);
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
