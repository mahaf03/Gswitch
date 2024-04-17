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
    SDL_Rect blockPositions[30]; // Antal block du vill ha
    int blockSpeed;
    int activeBlocks; // Antal aktiva block
    float playerSpeed;
    int playerLife; // Antal liv
    Uint32 startTime; // Starttid för timern
} GameModel;


void initializeModel(GameModel* model);
int min(int a, int b);
int checkCollision(SDL_Rect *shipRect, SDL_Rect *blockRect);
void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks);
void updateBlocks(GameModel* model, SDL_Rect shipRect);
#endif
