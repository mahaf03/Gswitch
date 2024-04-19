#include "GameModel.h"
#include "GameView.h"
#include <stdlib.h> // För rand() och srand()
#include <time.h>   // För time()

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750
#define SHIP_WIDTH 50
#define SHIP_HEIGHT 50



void initializeModel(GameModel* model) {
    srand((unsigned int)time(NULL));
    model->x = 200;
    model->y = 200;
    model->velocityX = model->velocityY = 0;
    model->up = model->down = model->left = model->right = false;
    model->collisionUp = model->collisionDown = model->collisionLeft = model->collisionRight = false;
    model->blockSpeed = 5;
    model->playerSpeed = 4.0f;
    model->playerLife = 5;
    model->activeBlocks = 5; // Startar med 5 block
    model->startTime = SDL_GetTicks(); // Startar tidräknaren
    model->isImmortal = false;  // Initially not immortal
    model->immortalStartTime = 0;  // Reset time

    for (int i = 0; i < 30; i++) { // Förbereder alla möjliga block
        model->blockPositions[i].x = 1200 + i * 50;
        model->blockPositions[i].y = rand() % 750;
        model->blockPositions[i].w = 50;
        model->blockPositions[i].h = 50;
    }
}



void updateBlocks(GameModel* model, SDL_Rect shipRect) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = (currentTime - model->startTime) / 1000; // Konverterar ms till sekunder

    // Uppdatera antalet aktiva block baserat på tiden
    if (elapsedTime < 12) {
        model->activeBlocks = 5;    
    } else if (elapsedTime < 25) {
        model->activeBlocks = 10;
        model->playerSpeed = 5.0f;
        model->blockSpeed = 8;
    } else {
        model->activeBlocks = 20;
        model->playerSpeed = 8.0f;
        model->blockSpeed = 10;
    }

    handleCollision(model, shipRect, model->blockPositions, model->activeBlocks);

    for (int i = 0; i < model->activeBlocks; i++) {
        model->blockPositions[i].x -= model->blockSpeed;
        if (model->blockPositions[i].x < -model->blockPositions[i].w) {
            model->blockPositions[i].x = 1200;
            model->blockPositions[i].y = rand() % (750 - model->blockPositions[i].h);
        }
    }
}


int min(int a, int b) {
    return (a < b) ? a : b;
}

int checkCollision(SDL_Rect* a, SDL_Rect* b) {
    // Beräkna sidorna av rect A och B
    int leftA = a->x;
    int rightA = a->x + a->w;
    int topA = a->y;
    int bottomA = a->y + a->h;

    int leftB = b->x;
    int rightB = b->x + b->w;
    int topB = b->y;
    int bottomB = b->y + b->h;

    // Om någon av sidorna från A inte kolliderar med B
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
        return 0; // Ingen kollision
    }

    // Annars finns det en kollision
    return 1;
}

void handleCollision(GameModel* model, SDL_Rect shipRect, SDL_Rect* blockPositions, int numBlocks) {
    bool collisionDetected = false;

    for (int i = 0; i < numBlocks; i++) {
        if (checkCollision(&shipRect, &blockPositions[i])) {
            if (!model->isImmortal) {  // Only process collision if not immortal
                model->playerLife--; // Decrease life
                if (model->playerLife == 0) {
                    printf("Game Over\n");
                    exit(0);
                }
                
                // Set the player to be immortal for 2 seconds
                model->isImmortal = true;
                model->immortalStartTime = SDL_GetTicks();
            }


            // Hantera kollision genom att stoppa spelarens rörelse eller justera position
            // Exempel: Stoppa spelaren från att röra sig in i blocket
            if (shipRect.y < blockPositions[i].y) {
                // Kollision underifrån
                model->y = blockPositions[i].y - shipRect.h;
                model->collisionDown = true;
            } else if (shipRect.y > blockPositions[i].y) {
                // Kollision ovanifrån
                model->y = blockPositions[i].y + blockPositions[i].h;
                model->collisionUp = true;
            }

            if (shipRect.x < blockPositions[i].x) {
                // Kollision till höger
                model->x = blockPositions[i].x - shipRect.w;
                model->collisionRight = true;
            } else if (shipRect.x > blockPositions[i].x) {
                // Kollision till vänster
                model->x = blockPositions[i].x + blockPositions[i].w;
                model->collisionLeft = true;
            }
            
        }
    }

    if (!collisionDetected) {
        model->collisionRight = false;
        model->collisionLeft = false;
        model->collisionUp = false;
        model->collisionDown = false;
    }
}

void updateGameState(GameModel* model) {
    updateCharacterPosition(model);
    Uint32 currentTime = SDL_GetTicks();
    if (model->isImmortal && (currentTime - model->immortalStartTime >= 3000)) {
        model->isImmortal = false;  // End immortality
    }

    // Continue with other game updates
}

void updateCharacterPosition(GameModel* model) {
    // Update position based on velocity
    model->x += model->velocityX;
    model->y += model->velocityY;

    // Clamp position to stay within the window
    if (model->x < 0) {
        model->x = 0;
        model->collisionLeft = true;  // Optional: Set collision flag if needed
    } else if (model->x > WINDOW_WIDTH - SHIP_WIDTH) {
        model->x = WINDOW_WIDTH - SHIP_WIDTH;
        model->collisionRight = true;  // Optional: Set collision flag if needed
    }

    if (model->y < 0) {
        model->y = 0;
        model->collisionUp = true;  // Optional: Set collision flag if needed
    } else if (model->y > WINDOW_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT) {
        model->y = WINDOW_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT;
        model->collisionDown = true;  // Optional: Set collision flag if needed
    }
}

