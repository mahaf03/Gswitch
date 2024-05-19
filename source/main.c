#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "Network.h"
#include "server.h"

void clearScreenAndStopMusic(SDL_Renderer *renderer, Mix_Music *music,
                             SDL_Texture **texture, SDL_Texture **texture1, SDL_Texture **bgTexture,
                             SDL_Texture **blockTexture, SDL_Texture **continueTexture,
                             SDL_Texture **exitTexture, SDL_Texture **volumeTexture, SDL_Texture **muteVolume)
{
    if (music != NULL)
    {
        Mix_HaltMusic();
    }

    SDL_Texture *textures[] = {*texture, *texture1, *bgTexture, *blockTexture, *continueTexture, *exitTexture, *volumeTexture, *muteVolume};
    for (int i = 0; i < sizeof(textures) / sizeof(SDL_Texture *); ++i)
    {
        if (textures[i] != NULL)
        {
            SDL_DestroyTexture(textures[i]);
            textures[i] = NULL;
        }
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argv, char **args)
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL; // Skeppets textur
    SDL_Texture *texture1 = NULL;
    SDL_Texture *bgTexture = NULL; // Bakgrundstextur
    SDL_Texture *youDiedTexture = NULL;
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

    Mix_Music *backgroundMusic = Mix_LoadMUS("resources/music2.mp3");
    if (backgroundMusic == NULL)
    {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Spela musik
    Mix_PlayMusic(backgroundMusic, -1); // Sista argumentet är antalet repetitioner (-1 för oändlig loop)

    menuView(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, &volumeTexture, &youDiedTexture);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta
    Uint32 frameStart;
    int frameTime;
    int playercount = 0;
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
    bool closeWindow = false;
    GameWindowState currentMusicState = MusicOn;
    GameWindowState currentState = Menu;

    float prePosX = model.player[0].x;
    float prePosY = model.player[0].y;
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
                        gameView(&renderer, &window, &texture, &texture1, &bgTexture, &blockTexture);
                        currentState = waitForPlayers;
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
                if (currentState == Game)
                {
                    handleEvent(&event, &model.player[0], &closeWindow);
                }
            }
        }

        if (currentState == Menu)
        {
            renderMenu(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, continueButtonRect, exitButtonRect, volumeButtonRect, &volumeTexture);
        }
        else if (currentState == waitForPlayers)
        {
            renderWaitForPlayers(&renderer, &window, &bgTexture);

            // Check network messages
            udpDataToClient message;
            IPaddress addrr = clientReceivePacket(&message, &sd);
            if (addrr.host != 0 && addrr.port != 0)
            {
                if (message.status == 1)
                {
                    currentState = Game;
                    printf("Game starting with 4 players!\n");
                }
                else
                {
                    // Update player positions
                    for (int i = 0; i < 4; i++)
                    {
                        if (message.player.playerID == model.player[i].playerID)
                        {
                            model.player[i].x = message.player.x;
                            model.player[i].y = message.player.y;
                            break;
                        }
                    }
                }
            }
        }
        else if (!model.alive)
        {
            printf("Player is dead\n");

            loadYouDiedTexture(renderer, &youDiedTexture);
            renderYouDied(&renderer, &window, &youDiedTexture);

            while (!closeWindow)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        closeWindow = true;
                    }
                }
                SDL_RenderClear(renderer);
                SDL_Delay(100);
            }
        }
        else if (currentState == Game)
        {
            SDL_DestroyTexture(continueTexture);
            SDL_DestroyTexture(exitTexture);

            for (int i = 0; i < 4; i++)
            {
                SDL_Rect shipRect = {(int)model.player[i].x, (int)model.player[i].y, 50, 50};
                updatePlayer(&model.player[i]);
            }

            SDL_Rect shipRect = {(int)model.player[0].x, (int)model.player[0].y, 50, 50};
            updateBlocks(&model, shipRect);
            updateGameState(&model);

            const float THRESHOLD = 1.f;
            float diffX = prePosX - model.player[0].x;
            float diffY = prePosY - model.player[0].y;
            float squarediff = diffX * diffX + diffY * diffY;
            if (1 / invSqrt(squarediff) > THRESHOLD)
            {
                udpDataToServer testdata = {model.player[0], 0};
                clientSendPacket(testdata, &srvadd, &sd);

                prePosX = model.player[0].x;
                prePosY = model.player[0].y;
            }

            renderView(renderer, texture, texture1, bgTexture, blockTexture, &model, shipRect, youDiedTexture);
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    Mix_FreeMusic(backgroundMusic);
    backgroundMusic = NULL;

    Mix_CloseAudio();

    closeView(renderer, window, texture, bgTexture, blockTexture);
    SDL_Quit();
    return 0;
}