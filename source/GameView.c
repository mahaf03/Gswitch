// GameView.c
#include <SDL2/SDL_image.h>
#include "GameView.h"
#include <stdbool.h>
#include "GameController.h"
#include "GameModel.h"
void menuView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture)
{
    SDL_Surface *surface = IMG_Load("resources/background.png");
    if (!surface)
    {
        printf("Unable to load background image: %s\n", IMG_GetError());
        *backgroundTexture = NULL;
        return;
    }
    *backgroundTexture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    // Load "Continue" button image
    SDL_Surface *continueSurface = IMG_Load("resources/continue.png");
    if (!continueSurface)
    {
        printf("Unable to load continue button image: %s\n", IMG_GetError());
        *continueTexture = NULL;
    }
    else
    {
        *continueTexture = SDL_CreateTextureFromSurface(*renderer, continueSurface);
        SDL_FreeSurface(continueSurface);
    }

    SDL_Surface *exitSurface = IMG_Load("resources/exit.png");
    if (!exitSurface)
    {
        printf("Unable to load exit button image: %s\n", IMG_GetError());
        *exitTexture = NULL;
    }
    else
    {
        *exitTexture = SDL_CreateTextureFromSurface(*renderer, exitSurface);
        SDL_FreeSurface(exitSurface);
    }
}
void renderMenu(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Rect continueButtonRect, SDL_Rect exitButtonRect)
{
    SDL_RenderClear(*renderer);

    // Render the background
    if (*backgroundTexture)
    {
        SDL_RenderCopy(*renderer, *backgroundTexture, NULL, NULL);
    }

    // Render the "Continue" button
    if (*continueTexture)
    {
        SDL_RenderCopy(*renderer, *continueTexture, NULL, &continueButtonRect);
    }

    // Render the "Exit" button
    if (*exitTexture)
    {
        SDL_RenderCopy(*renderer, *exitTexture, NULL, &exitButtonRect);
    }
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

void renderView(SDL_Renderer *renderer, SDL_Texture *shipTexture, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture, GameModel *model, SDL_Rect shipRect)
{
    SDL_RenderClear(renderer);

    // Antag att dessa är dina blockpositioner
    // SDL_Rect blockPositions[] = { Gammla blocken
    //     {0, 0, 50, 50}, {50, 0, 50, 50}, // Övre vänstra hörnet
    //     {0, 750, 50, 50}, {50, 750, 50, 50}, {100, 750, 50, 50},{150, 750, 50, 50}, // Nedre vänstra hörnet
    //     {150, 750, 50, 50}, {200, 750, 50, 50}, {250, 750, 50, 50}, // Fler positioner...
    //     {500, 750, 50, 50}, {550, 750, 50, 50}, {600, 750, 50, 50},
    //     {650, 750, 50, 50}, {700, 750, 50, 50},
    //     {600, 400, 50, 50}, {650, 400, 50, 50}, {700, 400, 50, 50}, {750, 400, 50, 50}, {800, 400, 50, 50},
    // };

    // int numBlocks = sizeof(blockPositions) / sizeof(blockPositions[0]);

    // Rendera bakgrunden
    if (backgroundTexture)
    {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Rendera skeppet
    // SDL_Rect shipRect = { (int)model->x, (int)model->y, 50, 50 };
    SDL_RenderCopy(renderer, shipTexture, NULL, &shipRect);

    //    handleCollision(model, shipRect, blockPositions, numBlocks, renderer, blockTexture); Gammla kollision med gamla blocken

    // Rendera blocken från listan
    // for (int i = 0; i < numBlocks; i++) {
    //     placeTile(renderer, blockTexture, blockPositions[i].x, blockPositions[i].y);
    // }
    for (int i = 0; i < 20; i++)
    {
        placeTile(renderer, blockTexture, model->blockPositions[i].x, model->blockPositions[i].y);
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
