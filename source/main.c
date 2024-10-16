#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "Network.h"
#include <SDL2/SDL_ttf.h>

// Funktion för att rendera text

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
    Player player;
    window = SDL_CreateWindow("GSwitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 686, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool gameReady = false;

    SDL_Rect continueButtonRect = {450, 150, 300, 100}; // Centrerad position och storlek för "Continue" knappen
    SDL_Rect exitButtonRect = {450, 300, 300, 100};
    SDL_Rect volumeButtonRect = {450, 450, 300, 100};
    SDL_Rect MuteButtonRect = {500, 430, 150, 150}; // Positionerad under "Continue" knappen med 50 pixels mellanrum

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

    menuView(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, &volumeTexture, &youDiedTexture);
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }
    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Tiden varje frame bör ta
    Uint32 frameStart;
    int frameTime;

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
    // initializeModel(&model);
    GameWindowState currentMusicState = MusicOn;
    GameWindowState currentState = Ip;

    float prePosX = model.player[0].x;
    float prePosY = model.player[0].y;

    bool gameWon = false;
    int winnerId = 0; // Initialisera till 0 eller något giltigt spelvärde
    Uint32 winnerMenuStartTime = 0;
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
                        currentState = waitForPlayers;
                        udpDataToServer testdata = {model.player[0], 0};
                        clientSendPacket(testdata, &srvadd, &sd);
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
                else if (currentState == Ip)
                {

                    while (SDL_PollEvent(&event))
                    {
                        if (event.type == SDL_QUIT)
                        {
                            closeWindow = true;
                        }
                    }
                }
            }

            if (currentState == Ip)
            {
                char ipAddress[20];
                getInputIP(ipAddress, sizeof(ipAddress), renderer, &currentState, bgTexture);

                if (SDLNet_ResolveHost(&srvadd, ipAddress, 49156) == -1)
                {
                    fprintf(stderr, "SDLNet_ResolveHost(%s 49156): %s\n", ipAddress, SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }

                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        closeWindow = true;
                    }
                }
            }

            if (currentState == Game)
            {
                handleEvent(&event, &model.player[0], &closeWindow);
            }
        }
        if (currentState == Menu)
        {
            // Rendera menyn
            initializeModel(&model);
            renderMenu(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, continueButtonRect, exitButtonRect, volumeButtonRect, &volumeTexture);
        }
        else if (currentState == waitForPlayers)
        {
            // Rendera vänta på spelare-skärmen
            // initializeModel(&model);
            renderWaitForPlayers(&renderer, &window, &bgTexture);
            // Kolla om vi har fått tillräckligt med spelare
            if (gameReady)
            {
                currentState = Game;
                gameView(&renderer, &window, &texture, &texture1, &bgTexture, &blockTexture);
                model.gameState.startTime = SDL_GetTicks();
            }
        }

        else if (currentState == Game)
        {
            // Uppdatera och rendera spelet
            for (int i = 0; i < 4; i++)
            {
                SDL_Rect shipRect = {(int)model.player[i].x, (int)model.player[i].y, 50, 50};
                updatePlayer(&model.player[i]);
            }

            SDL_Rect shipRect = {(int)model.player[0].x, (int)model.player[0].y, 50, 50};
            updateBlocks(&model, shipRect);
            updateGameState(&model);

            /*NETWORKING*/
            const float THRESHOLD = 1.f;
            float diffX = prePosX - model.player[0].x;
            float diffY = prePosY - model.player[0].y;
            float squarediff = diffX * diffX + diffY * diffY;
            if (1 / invSqrt(squarediff) > THRESHOLD)
            {
                // Send data to server
                udpDataToServer testdata = {model.player[0], 0};
                clientSendPacket(testdata, &srvadd, &sd);

                prePosX = model.player[0].x;
                prePosY = model.player[0].y;
            }
            renderView(renderer, texture, texture1, bgTexture, blockTexture, &model, shipRect, youDiedTexture);
        }

        frameTime = SDL_GetTicks() - frameStart; // Hur länge det tog att processa ramen
        udpDataToClient message;
        IPaddress addrr = clientReceivePacket(&message, &sd);

        if (addrr.host != 0 && addrr.port != 0) // har vi tagit emot något??
        {
            printf("new message from %x:\n", addrr.host);
            printf("\tx:\t%f\n\ty:\t%f\n\tlife:%d\n}", message.player.x, message.player.y, message.player.playerLife);
            model.player[0].playerID = message.clientId; // assign this clients playerID
            gameReady = message.gameReady;
            memcpy(&model.environment.next30Rand, &message.next30Rand, sizeof(int) * 30);
            for (int i = 0; i < 4; i++)
            {
                if (message.player.playerID == model.player[0].playerID)
                {
                    // this is our message resent back to us by the server
                    break;
                }

                if (model.player[i].playerID == -1)
                {
                    // previously unknown player
                    model.player[i].playerID = message.player.playerID;
                }
                if (model.player[i].playerID == message.player.playerID)
                {
                    model.player[i].x = message.player.x;
                    model.player[i].y = message.player.y;
                    model.player[i].playerLife = message.player.playerLife;
                    model.player[i].isDead = message.player.isDead;
                    break;
                }
            }
        }

        // Kontrollera vinnare och spelets tillstånd
        int aliveCount = 0;
        int lastAlivePlayerId = -1;
        for (int i = 0; i < 4; i++)
        {
            if (!model.player[i].isDead)
            {
                aliveCount++;
                lastAlivePlayerId = model.player[i].playerID;
            }
        }

        if (aliveCount == 1 && !gameWon)
        {
            winnerId = lastAlivePlayerId;
            gameWon = true;
            winnerMenuStartTime = SDL_GetTicks();
        }

        if (aliveCount == 0 && gameWon)
        {
            currentState = winnerMenu;
            printf("Alla spelare är döda. Vinnare: Player %d\n", winnerId + 1);
        }

        if (currentState == winnerMenu)
        {
            renderWinnerMenu(renderer, winnerId);
            if (SDL_GetTicks() - winnerMenuStartTime >= 10000)
            { // Kontrollera om 5 sekunder har passerat
                gameWon = false;
                gameReady = false;
                currentState = Ip; // Byt tillstånd till huvudmenyn eller något annat lämpligt tillstånd

            } // Återställ vinnarflaggan'
            // När en specifik händelse inträffar, skicka återställningsförfrågan

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    closeWindow = true;
                }
            }
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
    TTF_Quit();
    return 0;
}
