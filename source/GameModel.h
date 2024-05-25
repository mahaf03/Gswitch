// GameModel.h
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <string.h>

typedef enum
{
    Ip,
    Menu,
    Game,
    MusicOn,
    MusicOff,
    YouDied,
    waitForPlayers, 
    waitingForGameToEnd,
    winnerMenu,
} GameWindowState;


typedef enum {
    ASTRONAUT,
    ASTRONAUT1
} PlayerImage;

typedef struct {
    float x, y;  // Player's position
    float velocityX, velocityY;  // Player's velocity
    bool up, down, left, right;  // Direction flags
    bool collisionUp, collisionDown, collisionLeft, collisionRight;  // Collision flags
    float playerSpeed;
    int playerLife; 
    bool isImmortal;
    Uint32 immortalStartTime;
    int playerID;
    bool isDead;
    PlayerImage currentPlayerImage;
} Player;

typedef struct {
    SDL_Rect blockPositions[30];  // Array of block positions
    int blockSpeed;
    int activeBlocks;
    int numRocks;
    int lastRockIncreaseTime;
    int next30Rand[30];
} Environment;

typedef struct {
    Uint32 startTime;
    Uint32 lifeSpawnTime;
    int lifePosX;
    int lifePosY;
    bool lifeActive;
} GameState;

typedef struct {
    Player player[4];
    Environment environment;
    GameState gameState;
    bool alive;
    int playercount;
} GameModel;

void initializeModel(GameModel* model);
int min(int a, int b);
int checkCollision(SDL_Rect *shipRect, SDL_Rect *blockRect);
void handleCollision(Player *player, SDL_Rect shipRect, SDL_Rect *blockPositions, int numBlocks, GameModel *model);
//void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks);
void updateBlocks(GameModel* model, SDL_Rect shipRect);
void updateGameState(GameModel* model);
void updateCharacterPosition(GameModel* model);

#endif
