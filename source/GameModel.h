// GameModel.h
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    float x, y;  // Player's position
    float velocityX, velocityY;  // Player's velocity
    bool up, down, left, right;  // Direction flags
    bool collisionUp, collisionDown, collisionLeft, collisionRight;  // Collision flags
    float playerSpeed;
    int playerLife; 
    bool isImmortal;
    Uint32 immortalStartTime;
} Player;

typedef struct {
    SDL_Rect blockPositions[30];  // Array of block positions
    int blockSpeed;
    int activeBlocks;
    int numRocks;
    int lastRockIncreaseTime;
} Environment;

typedef struct {
    Uint32 startTime;
    Uint32 lifeSpawnTime;
    int lifePosX;
    int lifePosY;
    bool lifeActive;
} GameState;


void initializeModel(GameModel* model);
int min(int a, int b);
int checkCollision(SDL_Rect *shipRect, SDL_Rect *blockRect);
void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks);
void updateBlocks(GameModel* model, SDL_Rect shipRect);
void updateGameState(GameModel* model);
void updateCharacterPosition(GameModel* model);

#endif
