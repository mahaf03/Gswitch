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
        model->blockPositions[i].x = 1200 + i * 50; // Startar till höger utanför skärmen
        model->blockPositions[i].y = rand() % 750; // Ett slumpmässigt y-värde
        model->blockPositions[i].w = 50;
        model->blockPositions[i].h = 50;
    }
}

void updateBlocks(GameModel* model, SDL_Rect shipRect) {
        // Hantera kollisioner för varje block
        handleCollision(model, shipRect, model->blockPositions, 20);
    
    for (int i = 0; i < 20; i++) {
        // Uppdatera x-positionen för varje block
        model->blockPositions[i].x -= model->blockSpeed;
        // Kontrollera om blocket har passerat skärmens vänstra kant
        if (model->blockPositions[i].x < -model->blockPositions[i].w) {
            // Återställ blocket till höger utanför skärmen
            model->blockPositions[i].x = 1200;
            // Ge det en ny y-position
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
            collisionDetected = true;

            // Hantera kollision genom att stoppa spelarens rörelse eller justera position
            // Exempel: Stoppa spelaren från att röra sig in i blocket
            if (shipRect.y < blockPositions[i].y) {
                // Kollision underifrån
                model->y = blockPositions[i].y - shipRect.h;
            } else if (shipRect.y > blockPositions[i].y) {
                // Kollision ovanifrån
                model->y = blockPositions[i].y + blockPositions[i].h;
            }

            if (shipRect.x < blockPositions[i].x) {
                // Kollision till höger
                model->x = blockPositions[i].x - shipRect.w;
            } else if (shipRect.x > blockPositions[i].x) {
                // Kollision till vänster
                model->x = blockPositions[i].x + blockPositions[i].w;
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


