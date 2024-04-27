// GameView.c
#include <SDL2/SDL_image.h>
#include "GameView.h"
#include <stdbool.h>
#include "GameController.h"
#include "GameModel.h"
void menuView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Texture **volumeTexture)
{
    SDL_Surface *surface = IMG_Load("resources/background.png");
    *backgroundTexture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Surface *continueSurface = IMG_Load("resources/continue.png");
    *continueTexture = SDL_CreateTextureFromSurface(*renderer, continueSurface);
    SDL_FreeSurface(continueSurface);

    SDL_Surface *exitSurface = IMG_Load("resources/exit.png");
    *exitTexture = SDL_CreateTextureFromSurface(*renderer, exitSurface);
    SDL_FreeSurface(exitSurface);

    SDL_Surface *volumeSurface = IMG_Load("resources/unmute.png");
    *volumeTexture = SDL_CreateTextureFromSurface(*renderer, volumeSurface);
    SDL_FreeSurface(volumeSurface); // Korrekt frigör ytan
}
void renderMenu(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Rect continueButtonRect, SDL_Rect exitButtonRect, SDL_Rect volumeButtonRect, SDL_Texture **volumeTexture)
{
    SDL_RenderClear(*renderer);
    SDL_RenderCopy(*renderer, *backgroundTexture, NULL, NULL);
    SDL_RenderCopy(*renderer, *volumeTexture, NULL, &volumeButtonRect);
    SDL_RenderCopy(*renderer, *continueTexture, NULL, &continueButtonRect);
    SDL_RenderCopy(*renderer, *exitTexture, NULL, &exitButtonRect);
    SDL_RenderPresent(*renderer);
}

void gameView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **texture, SDL_Texture **backgroundTexture, SDL_Texture **blockTexture)
{

    SDL_Surface *surface = IMG_Load("resources/austrounat.png");
    *texture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    // Ladda bakgrundstexturen
    loadBackground(*renderer, backgroundTexture);
    loadBlock(*renderer, blockTexture);
}

void drawLives(SDL_Renderer *renderer, int lives, int x, int y)
{
    int barWidth = 15;
    SDL_Color color = {0, 255, 255}; // blå färg för livsindikatorn
    for (int i = 0; i < lives; i++)
    {
        SDL_Rect rect = {x + i * (barWidth + 5), y, barWidth, 20};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
void loadBackground(SDL_Renderer *renderer, SDL_Texture **backgroundTexture)
{
    SDL_Surface *surface = IMG_Load("resources/background.png");
    if (!surface)
    {
        printf("Unable to load background image: %s\n", IMG_GetError());
        *backgroundTexture = NULL;
        return;
    }
    *backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void loadBlock(SDL_Renderer *renderer, SDL_Texture **blockTexture)
{
    SDL_Surface *surface = IMG_Load("resources/speedTile3.png");
    if (!surface)
    {
        printf("Unable to load block image: %s\n", IMG_GetError());
        *blockTexture = NULL;
        return;
    }
    *blockTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void renderView(SDL_Renderer *renderer, SDL_Texture *shipTexture, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture, GameModel *model,Player* player, SDL_Rect shipRect)
{
    SDL_RenderClear(renderer);
    

    if (backgroundTexture)
    {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    SDL_Rect shipRectPlayers[4];

    for (int i = 0; i < 4; i++) {
        SDL_Rect shipRectPlayersI = { (int)model->player[i].x, (int)model->player[i].y, 50, 50 };
        shipRectPlayers[i] = shipRectPlayersI;
        SDL_RenderCopy(renderer, shipTexture, NULL, &shipRectPlayers[i]);
    }



    for (int i = 0; i < 20; i++)
    {
        placeTile(renderer, blockTexture, model->environment.blockPositions[i].x, model->environment.blockPositions[i].y);
        drawLives(renderer, player->playerLife, 10, 10); // Visa rektanglar som liv nedanför texten
    }

    if (model->gameState.lifeActive){
        drawExtraLife(renderer, model->gameState.lifePosX,model->gameState.lifePosY);
    }
    
    

    SDL_RenderPresent(renderer);
}

void closeView(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *shipTexture, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture)
{
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyTexture(backgroundTexture); // Rensa bakgrundstexturen
    SDL_DestroyTexture(blockTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void placeTile(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect tileRect = {x, y, 50, 50}; // Antag att varje tile är 50x50 pixlar
    SDL_RenderCopy(renderer, texture, NULL, &tileRect);
}

void drawExtraLife(SDL_Renderer* renderer, int x, int y) {
    int barWidth = 15;
    SDL_Color color = {255, 0, 0};
    //printf("x: %d, y: %d\n", x, y);
    SDL_Rect rect = {x, y, barWidth, 20};
    //SDL_Rect rect = {20, 20, 20, 20};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
    //printf("Extra life\n");
    
}


