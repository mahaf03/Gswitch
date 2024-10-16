#include "GameModel.h"
#include "GameView.h"
#include <stdlib.h> // För rand() och sra(
#include <time.h>   // För time()

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750
#define SHIP_WIDTH 50
#define SHIP_HEIGHT 50

PriorityMessage queue[100];
int queueSize = 0;



void initializeModel(GameModel *model)
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 4; i++)
    {
        model->player[i].x = 400;
        model->player[i].y = 350;
        model->player[i].velocityX = model->player[i].velocityY = 0;
        model->player[i].up = model->player[i].down = model->player[i].left = model->player[i].right = false;
        model->player[i].collisionUp = model->player[i].collisionDown = model->player[i].collisionLeft = model->player[i].collisionRight = false;
        model->player[i].playerSpeed = 4.0f;
        model->player[i].isImmortal = false;    // Initially not immortal
        model->player[i].immortalStartTime = 0; // Reset time
        model->player[i].playerLife = 4;
        model->player[i].playerID = -1;
    }
    model->environment.blockSpeed = 5;
    model->environment.activeBlocks = 5;         // Startar med 5 block
    model->gameState.startTime = SDL_GetTicks(); // Startar tidräknaren
    model->gameState.lifeSpawnTime = SDL_GetTicks();
    model->gameState.lifeActive = false;
    model->gameState.lifePosX = 0;
    model->gameState.lifePosY = 0;
    model->alive = true;
    for (int i = 0; i < 30; i++)
    { // Förbereder alla möjliga block
        model->environment.blockPositions[i].x = 1200 + 15 * 50;
        model->environment.blockPositions[i].y = model->environment.next30Rand[i];;
        model->environment.blockPositions[i].w = 50;
        model->environment.blockPositions[i].h = 50;
    }
}

void updateBlocks(GameModel *model, SDL_Rect shipRect)
{
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = (currentTime - model->gameState.startTime) / 1000; // Konverterar ms till sekunder

    // Uppdatera antalet aktiva block baserat på tiden
    if (elapsedTime < 12)
    {
        model->environment.activeBlocks = 5;
    }
    else if (elapsedTime < 25)
    {
        model->environment.activeBlocks = 10;
        for (int i = 0; i < 4; i++)
        {
            model->player[i].playerSpeed = 5.0f;
        }
        model->environment.blockSpeed = 8;
    }
    else
    {
        model->environment.activeBlocks = 20;
        for (int i = 0; i < 4; i++)
        {
            model->player[i].playerSpeed = 8.0f;
        }
        model->environment.blockSpeed = 10;
    }
    for (int i = 0; i < 4; i++)
    {
        handleCollision(&model->player[i], shipRect, model->environment.blockPositions, model->environment.activeBlocks, model);
    }

    for (int i = 0; i < model->environment.activeBlocks; i++)
    {
        model->environment.blockPositions[i].x -= model->environment.blockSpeed;
        if (model->environment.blockPositions[i].x < -model->environment.blockPositions[i].w)
        {
            //model->environment.blockPositions[i].y = rand() % (750 - model->environment.blockPositions[i].h);
            model->environment.blockPositions[i].y = model->environment.next30Rand[i];
            model->environment.blockPositions[i].x = 1200 + 77*( model->environment.blockPositions[i].y % 5);
            //
        }
    }
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int checkCollision(SDL_Rect *a, SDL_Rect *b)
{
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
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
    {
        return 0; // Ingen kollision
    }

    // Annars finns det en kollision
    return 1;
}

void handleCollision(Player *player, SDL_Rect shipRect, SDL_Rect *blockPositions, int numBlocks, GameModel *model)
{
    bool collisionDetected = false;
    if (!player->isImmortal)
    {
        // Skip collision detection if immortal
        for (int i = 0; i < numBlocks; i++)
        {
            if (checkCollision(&shipRect, &blockPositions[i]))
            {
                if (!player->isImmortal)
                {                         // Only process collision if not immortal
                    player->playerLife--; // Decrease life
                    if (player->playerLife == 0)
                    {
                        printf("Player %d died\n", player->playerID);
                        model->playercount--;
                        player->isDead = true;
                        void handlePlayerDeath(int playerId, CriticalUpdate criticalUpdate);
                        // Exempel på att lägga till ett kritiskt meddelande i kön
                        

                        // model->alive = false;
                        //*gameState = YouDied;
                        //exit(0);

                    }

                    // Set the player to be immortal for 2 seconds
                    player->isImmortal = true;
                    player->immortalStartTime = SDL_GetTicks();
                }

                // Hantera kollision genom att stoppa spelarens rörelse eller justera position
                // Exempel: Stoppa spelaren från att röra sig in i blocket
                if (shipRect.y < blockPositions[i].y)
                {
                    // Kollision underifrån
                    player->y = blockPositions[i].y - shipRect.h;
                    player->collisionDown = true;
                }
                else if (shipRect.y > blockPositions[i].y)
                {
                    // Kollision ovanifrån
                    player->y = blockPositions[i].y + blockPositions[i].h;
                    player->collisionUp = true;
                }

                if (shipRect.x < blockPositions[i].x)
                {
                    // Kollision till höger
                    player->x = blockPositions[i].x - shipRect.w;
                    player->collisionRight = true;
                }
                else if (shipRect.x > blockPositions[i].x)
                {
                    // Kollision till vänster
                    player->x = blockPositions[i].x + blockPositions[i].w;
                    player->collisionLeft = true;
                }
            }
        }
    }

    if (!collisionDetected)
    {
        player->collisionRight = false;
        player->collisionLeft = false;
        player->collisionUp = false;
        player->collisionDown = false;
    }
}

void updateGameState(GameModel *model)
{
    updateCharacterPosition(model);
    Uint32 currentTime = SDL_GetTicks();
    for (int i = 0; i < 4; i++)
    {
        if (model->player[i].isImmortal && (currentTime - model->player[i].immortalStartTime >= 3000))
        {
            model->player[i].isImmortal = false; // End immortality
        }
    }
    // printa meddelande varje 4 sekunder

    if (currentTime - model->gameState.lifeSpawnTime >= 4000)
    { // ändra pos för life på interval
        model->gameState.lifeActive = true;
        model->gameState.lifePosX = rand() % WINDOW_WIDTH;
        model->gameState.lifePosY = rand() % WINDOW_HEIGHT;
        model->gameState.lifeSpawnTime = currentTime;
    }

    // Todo gör lifeActive till flase om spelaren kolliderar med den

    // Continue with other game updates
}

void updateCharacterPosition(GameModel *model)
{
    // Update position based on velocity
    for (int i = 0; i < 4; i++)
    {
        model->player[i].x += model->player[i].velocityX;
        model->player[i].y += model->player[i].velocityY;

        // Clamp position to stay within the window

        if (model->player[i].x < 0)
        {
            model->player[i].x = 0;
        }
        else if (model->player[i].x > WINDOW_WIDTH - SHIP_WIDTH)
        {
            model->player[i].x = WINDOW_WIDTH - SHIP_WIDTH;
        }
        if (model->player[i].y < 0)
        {
            model->player[i].y = 0;
        }
        else if (model->player[i].y > WINDOW_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT / 2.f)
        {
            model->player[i].y = WINDOW_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT - SHIP_HEIGHT / 2.f;
        }
    }
}

void handlePlayerDeath(int playerId, CriticalUpdate criticalUpdate) {
    // Ange att spelaren har dött
    criticalUpdate.playerId = playerId;
    criticalUpdate.gameState = waitingForGameToEnd;  // Antag att detta är en konstant som representerar spelets tillstånd
    // Prioritera och skicka meddelandet
    enqueueMessage(PRIORITY_HIGH, &criticalUpdate, sizeof(CriticalUpdate));
}

void enqueueMessage(MessagePriority priority, const void* message, size_t size) {
    if (queueSize < 100) {
        queue[queueSize].priority = priority;
        queue[queueSize].message = malloc(size);
        memcpy(queue[queueSize].message, message, size);
        queue[queueSize].messageSize = size;
        queueSize++;
        // Sortera kön baserat på prioritet, högsta prioritet först
        // Detta är en mycket enkel sortering för demonstration, överväg att använda effektivare algoritmer
        for (int i = 0; i < queueSize; i++) {
            for (int j = i + 1; j < queueSize; j++) {
                if (queue[j].priority < queue[i].priority) {
                    PriorityMessage temp = queue[i];
                    queue[i] = queue[j];
                    queue[j] = temp;
                }
            }
        }
    }
}