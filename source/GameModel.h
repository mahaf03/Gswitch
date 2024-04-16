// GameModel.h
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    float x, y; // Player's position
    float velocityX, velocityY; // Player's velocity
    bool up, down, left, right; // Direction flags
    bool collisionUp, collisionDown, collisionLeft, collisionRight; // Collision flags
} GameModel;

void initializeModel(GameModel* model);
int min(int a, int b);
int checkCollision(SDL_Rect shipRect, SDL_Rect blockRect);
void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks, SDL_Renderer* renderer, SDL_Texture* blockTexture);

#endif
