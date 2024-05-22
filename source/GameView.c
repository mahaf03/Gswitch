// GameView.c
#include <SDL2/SDL_image.h>
#include "GameView.h"
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "GameController.h"
#include "GameModel.h"

SDL_Texture *renderText(const char *message, const char *fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont(fontFile, fontSize);
    if (!font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
    if (!surf)
    {
        TTF_CloseFont(font);
        printf("TTF_RenderText: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!texture)
    {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

// Funktion för att få IP-adress från användaren
// Funktion för att få IP-adress från användaren
void getInputIP(char *ipBuffer, int bufferSize, SDL_Renderer *renderer, GameWindowState *currentState, SDL_Texture *backgroundTexture)
{
    SDL_StartTextInput();
    bool done = false;
    SDL_Event e;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Rect inputRect = {400, 300, 400, 50};   // Justera detta efter behov
    SDL_Rect messageRect = {400, 250, 400, 50}; // Position och storlek för meddelandetexten
    char inputText[bufferSize];
    strcpy(inputText, "");

    // Skapa texturen för meddelandet "Enter IP Address"
    SDL_Texture *messageTexture = renderText("Enter IP Address", "resources/lato-italic.ttf", textColor, 24, renderer);

    while (!done)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                done = true;
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                if (strlen(inputText) + strlen(e.text.text) < bufferSize - 1)
                {
                    strcat(inputText, e.text.text);
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
                {
                    inputText[strlen(inputText) - 1] = '\0';
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    strcpy(ipBuffer, inputText);
                    done = true;
                    *currentState = Menu; // Uppdatera spelets tillstånd till Menu
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Rendera bakgrundsbilden
        if (backgroundTexture)
        {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        }

        // Rendera meddelandetexten
        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

        // Rendera inmatningstexten
        SDL_Texture *textTexture = renderText(inputText, "resources/lato-italic.ttf", textColor, 24, renderer);
        SDL_RenderCopy(renderer, textTexture, NULL, &inputRect);

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(textTexture);
    }

    SDL_StopTextInput();
    SDL_DestroyTexture(messageTexture);
}

void menuView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Texture **volumeTexture, SDL_Texture **youDiedTexture)
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

    SDL_Surface *volumeSurface = IMG_Load("resources/mute.png");
    *volumeTexture = SDL_CreateTextureFromSurface(*renderer, volumeSurface);
    SDL_FreeSurface(volumeSurface); // Korrekt frigör ytan
}

void renderWaitForPlayers(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture)
{
    SDL_Color textColor = {255, 255, 255, 255}; // White color for the text
    SDL_Rect messageRect = {400, 300, 400, 50}; // Position and size for the message text

    // Create the texture for the message "Waiting for players"
    SDL_Texture *messageTexture = renderText("Waiting for players", "resources/lato-italic.ttf", textColor, 24, *renderer);

    SDL_RenderClear(*renderer);
    SDL_RenderCopy(*renderer, *backgroundTexture, NULL, NULL);
    SDL_RenderCopy(*renderer, messageTexture, NULL, &messageRect);
    SDL_RenderPresent(*renderer);
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
// Render you died screen

void loadYouDiedTexture(SDL_Renderer *renderer, SDL_Texture **youDiedTexture)
{
    SDL_Surface *surface = IMG_Load("resources/youdied.png");
    if (!surface)
    {
        printf("Unable to load you died image: %s\n", IMG_GetError());
        *youDiedTexture = NULL;
        return;
    }
    printf("You Died Texture loaded successfully\n");
    *youDiedTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void renderYouDied(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **youDiedTexture)
{
    if (*youDiedTexture == NULL)
    {
        printf("You Died texture is NULL\n");
        return;
    }

    SDL_RenderClear(*renderer);
    SDL_RenderCopy(*renderer, *youDiedTexture, NULL, NULL);
    SDL_RenderPresent(*renderer);
    printf("Rendered You Died texture\n");
}

void gameView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **texture, SDL_Texture **texture1, SDL_Texture **backgroundTexture, SDL_Texture **blockTexture)

{
    SDL_Surface *surface = IMG_Load("resources/austrounat.png");
    *texture = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resources/austrounat1.png");
    *texture1 = SDL_CreateTextureFromSurface(*renderer, surface);
    SDL_FreeSurface(surface);

    // Ladda bakgrundstexturen
    loadBackground(*renderer, backgroundTexture);
    // loadYouDiedTexture(renderer, youDiedTexture);
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
void renderView(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Texture *texture1, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture, GameModel *model, SDL_Rect shipRect, SDL_Texture *YouDiedTexture)
{
    SDL_RenderClear(renderer);

    SDL_Texture *currentTexture = (model->player[0].currentPlayerImage == ASTRONAUT1) ? texture1 : texture;

    if (backgroundTexture)
    {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    if (YouDiedTexture)
    {
        SDL_RenderCopy(renderer, YouDiedTexture, NULL, NULL);
    }

    SDL_Rect shipRectPlayers[4];
    for (int i = 0; i < 4; i++)
    {
        SDL_Rect shipRectPlayersI = {(int)model->player[i].x, (int)model->player[i].y, 50, 50};
        shipRectPlayers[i] = shipRectPlayersI;
        SDL_RenderCopy(renderer, currentTexture, NULL, &shipRectPlayers[i]);
    }

    SDL_RenderCopy(renderer, currentTexture, NULL, &shipRect);
    for (int i = 0; i < model->environment.activeBlocks; i++)
    {
        // placeTile(renderer, texture, model->environment.blockPositions[i].x, model->environment.blockPositions[i].y);
        placeTile(renderer, blockTexture, model->environment.blockPositions[i].x, model->environment.blockPositions[i].y);
    }

    drawLives(renderer, model->player[0].playerLife, 10, 10); // Visa rektanglar som liv nedanför texten

    if (model->gameState.lifeActive)
    {
        //    drawExtraLife(renderer, model->gameState.lifePosX, model->gameState.lifePosY);
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

void getInputName(char *nameBuffer, int bufferSize, SDL_Renderer *renderer, GameWindowState *currentState, SDL_Texture *backgroundTexture)
{
    SDL_StartTextInput();
    bool done = false;
    SDL_Event e;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Rect inputRect = {400, 300, 400, 50};   // Justera detta efter behov
    SDL_Rect messageRect = {400, 250, 400, 50}; // Position och storlek för meddelandetexten
    char inputText[bufferSize];
    strcpy(inputText, "");

    // Skapa texturen för meddelandet "Enter Your Name"
    SDL_Texture *messageTexture = renderText("Enter Your Name", "resources/lato-italic.ttf", textColor, 24, renderer);

    while (!done)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                done = true;
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                if (strlen(inputText) + strlen(e.text.text) < bufferSize - 1)
                {
                    strcat(inputText, e.text.text);
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
                {
                    inputText[strlen(inputText) - 1] = '\0';
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    strcpy(nameBuffer, inputText);
                    done = true;
                    *currentState = Menu; // Uppdatera spelets tillstånd till Menu
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Rendera bakgrundsbilden
        if (backgroundTexture)
        {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        }

        // Rendera meddelandetexten
        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

        // Rendera inmatningstexten
        SDL_Texture *textTexture = renderText(inputText, "resources/lato-italic.ttf", textColor, 24, renderer);
        SDL_RenderCopy(renderer, textTexture, NULL, &inputRect);

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(textTexture);
    }

    SDL_StopTextInput();
    SDL_DestroyTexture(messageTexture);
}


// void drawExtraLife(SDL_Renderer* renderer, int x, int y) {
//     int barWidth = 15;
//     SDL_Color color = {255, 0, 0};
//     //printf("x: %d, y: %d\n", x, y);
//     SDL_Rect rect = {x, y, barWidth, 20};
//     //SDL_Rect rect = {20, 20, 20, 20};
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
//     SDL_RenderFillRect(renderer, &rect);
//     //printf("Extra life\n");

// }
