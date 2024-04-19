#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "Network.h"

typedef enum
{
    Menu,
    Game,
    MusicOn,
    MusicOff

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
    SDL_Texture *volumeTexture = NULL;
    SDL_Texture *muteVolume = NULL;

    GameModel model;
    window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 686, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect continueButtonRect = {470, 100, 200, 200}; // Centrerad position och storlek för "Continue" knappen
    SDL_Rect exitButtonRect = {500, 250, 150, 200};
    SDL_Rect volumeButtonRect = {500, 430, 150, 150};
    SDL_Rect MuteButtonRect = {500, 430, 150, 150}; // Positionerad under "Continue" knappen med 50 pixels mellanrum

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

    menuView(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, &volumeTexture);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta
    Uint32 frameStart;
    int frameTime;
    GameModel models[4];
    int playercount = 0;
    for (int i = 0; i < 4; i++)
    {
        initializeModel(&models[i]);
    }
    // Du måste också passera en pekare till bakgrundstexturen till initView
    // initView(&renderer, &window, &texture, &bgTexture);
    printf("initializing network... \n");
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *pReceive;
    UDPpacket *p;
    initNetwork_Client(&sd, &srvadd, pReceive);
    printf("Network initialized!\n");
    // Allocate memory for UDP packets
    if (!(p = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (!((pReceive = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    bool closeWindow = false;
    GameState currentState = Menu;
    GameState currentMusicState = MusicOn;

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
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (currentState == Menu)
                {
                    if (mouseX >= continueButtonRect.x && mouseX <= continueButtonRect.x + continueButtonRect.w &&
                        mouseY >= continueButtonRect.y && mouseY <= continueButtonRect.y + continueButtonRect.h)
                    {
                        printf("Pressed continue button\n");
                        currentState = Game;
                    }
                    else if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                             mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h)
                    {
                        printf("Pressed exit button\n");
                        closeWindow = true;
                    }
                    else if (mouseX >= volumeButtonRect.x && mouseX <= volumeButtonRect.x + volumeButtonRect.w &&
                             mouseY >= volumeButtonRect.y && mouseY <= volumeButtonRect.y + volumeButtonRect.h)
                    {
                        if (currentMusicState == MusicOn)
                        {
                            Mix_PauseMusic();
                            currentMusicState = MusicOff;
                        }
                        else
                        {
                            Mix_ResumeMusic();
                            currentMusicState = MusicOn;
                        }
                    }
                }
            }
            if (currentState == Game)
            {
                handleEvent(&event, &model, &closeWindow);
                udpDataToServer testdata = {model.x, model.y, 0};
                if (model.x >= 400.f)
                {
                    testdata.status = 3;
                }
                clientSendPacket(testdata, &srvadd, &sd);
            }
        }
        if (currentState == Menu)
        {
            // Rendera menyn
            renderMenu(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, continueButtonRect, exitButtonRect, volumeButtonRect, &volumeTexture);
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
            updateGameState(&model);
            renderView(renderer, texture, bgTexture, blockTexture, &model, shipRect);
        }

        frameTime = SDL_GetTicks() - frameStart; // Hur länge det tog att processa ramen

        udpDataToClient message;
        IPaddress addrr = clientReceivePacket(&message, &sd);
        if (addrr.host != 0 && addrr.port != 0)
        {
            printf("new message from %x:\n", addrr.host);
            playercount = message.playercount;
            for (int i = 0; i < message.playercount; i++)
            {
                printf("\tplayer %d at %f %f\n", i, message.playerPositions[i].x, message.playerPositions[i].y);
                models[i].x = message.playerPositions[i].x;
                models[i].y = message.playerPositions[i].y;
            }
        }
        for (int i = 0; i < playercount; i++)
        {
            models[i].x = message.playerPositions[i].x;
            models[i].y = message.playerPositions[i].y;
            updateModel(&models[i]);
        }

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
