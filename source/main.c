#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
typedef enum
{
    Menu,
    Game

} GameState;
int main(int argv, char **args)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;      // Skeppets textur
    SDL_Texture *bgTexture = NULL;    // Bakgrundstextur
    SDL_Texture *blockTexture = NULL; // Blockets textur
    SDL_Texture *continueTexture = NULL;
    SDL_Texture *exitTexture = NULL;
    GameModel model;
    // Load "Continue" button image
    // SDL_Surface *continueSurface = IMG_Load("resources/continue.png");
    // SDL_Texture *continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
    // SDL_FreeSurface(continueSurface);

    // SDL_Surface *exitSurface = IMG_Load("resources/exit.png");
    // SDL_Texture *exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    // SDL_FreeSurface(exitSurface);
    window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 686, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect continueButtonRect = {500, 243, 200, 200}; // Centrerad position och storlek för "Continue" knappen
    SDL_Rect exitButtonRect = {550, 493, 100, 100};     // Positionerad under "Continue" knappen med 50 pixels mellanrum

    initializeModel(&model);

    bool success = true;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    Mix_Music *backgroundMusic = Mix_LoadMUS("resources/music.mp3");
    if (backgroundMusic == NULL)
    {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Spela musik
    Mix_PlayMusic(backgroundMusic, -1); // Sista argumentet är antalet repetitioner (-1 för oändlig loop)

    menuView(&renderer, &window, &bgTexture, &continueTexture, &exitTexture);

    // renderMenu(renderer, window, bgTexture, continueTexture, exitTexture, continueButtonRect, exitButtonRect);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta

    Uint32 frameStart;
    int frameTime;

    bool closeWindow = false;
    GameState currentState = Menu;

    while (!closeWindow)
    {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                closeWindow = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Kontrollera om användaren klickar på "Continue"
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (currentState == Menu &&
                    mouseX >= continueButtonRect.x && mouseX <= continueButtonRect.x + continueButtonRect.w &&
                    mouseY >= continueButtonRect.y && mouseY <= continueButtonRect.y + continueButtonRect.h)
                {
                    printf("Pressed contniue button\n");
                    currentState = Game; // Ändra tillstånd till Game
                }
                else if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                         mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h)
                {
                    printf("Pressed exit button\n");
                    closeWindow = true;
                }
            }
            if (currentState == Game)
            {
                handleEvent(&event, &model, &closeWindow);
            }
        }

        if (currentState == Menu)
        {
            // Rendera menyn
            renderMenu(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, continueButtonRect, exitButtonRect);
        }
        else if (currentState == Game)
        {
            // Uppdatera och rendera spelet
            SDL_DestroyTexture(continueTexture);
            SDL_DestroyTexture(exitTexture);
            gameView(&renderer, &window, &texture, &bgTexture, &blockTexture);
            SDL_Rect shipRect = {(int)model.x, (int)model.y, 50, 50};
            updateModel(&model);
            updateBlocks(&model, shipRect);
            renderView(renderer, texture, bgTexture, blockTexture, &model, shipRect);
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }


    Mix_FreeMusic(backgroundMusic); // För musik
    backgroundMusic = NULL;

    Mix_CloseAudio(); // Vid avslutning av programmet

    closeView(renderer, window, texture, bgTexture, blockTexture);
    SDL_Quit();
    return 0;
}

  