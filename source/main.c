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
} GameStates;

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
    for (int i=0; i<4;i++)
        {
            strcpy(model.player[i].playerName,"");
        }
    strcpy(model.player[0].playerName,*args);

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

    menuView(&renderer, &window, &bgTexture, &continueTexture, &exitTexture, &volumeTexture);

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
    GameStates currentMusicState = MusicOn;
    GameStates currentState = Menu;

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
                        gameView(&renderer, &window, &texture, &bgTexture, &blockTexture);
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
                handleEvent(&event, &model.player[0], &closeWindow);
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

            //Move players
            for(int i = 0; i < 4; i++){
                SDL_Rect shipRect = {(int)model.player[i].x, (int)model.player[i].y, 50, 50};
                updatePlayer(&model.player[i]);
            }


            /*NETWORKING*/
            const float THRESHOLD = 5.f;
            float diffX = prePosX - model.player[0].x;
            float diffY = prePosY - model.player[0].y;
            float squarediff = diffX*diffX + diffY*diffY;
            if( 1/invSqrt(squarediff) > THRESHOLD)
                {
                    //Send data to server
                    udpDataToServer testdata = {model.player[0], 0};
                    clientSendPacket(testdata, &srvadd, &sd);

                    /*utskrift*/
                  prePosX = model.player[0].x;
                   prePosY = model.player[0].y;
                    //printf("sent data to server \n\tx: %f\n\ty: %f\n",prePosX,prePosY);
                }

            SDL_Rect shipRect = {(int)model.player[0].x, (int)model.player[0].y, 50, 50};
            //updateBlocks(&model, shipRect);
            updateGameState(&model);
            // if move, then send to server.
            renderView(renderer, texture, bgTexture, blockTexture, &model, &model.player[0], shipRect);
        }

        frameTime = SDL_GetTicks() - frameStart; // Hur länge det tog att processa ramen
        udpDataToClient message;
        IPaddress addrr = clientReceivePacket(&message, &sd);
        clientReceivePacket(&message, &sd);
        if (addrr.host != 0 && addrr.port != 0) // har vi tagit emot något??
        {
            printf("new message from %x:\n", addrr.host);
            printf("\tx:\t%f\n\ty:\t%f\n}",message.player.x,message.player.y);
            for (int i=0 ;i<4;i++)
                {
                    if (strcmp(model.player[i].playerName,message.player.playerName))
                        {
                            model.player[i] =  message.player;
                            model.player[i].velocityX = 0.f;
                            model.player[i].velocityY = 0.f;
                            model.player[i].up= 0;
                            model.player[i].down= 0;
                            model.player[i].right= 0;
                            model.player[i].left= 0;
                            break;
                        }
                }

            /*
            //playercount = message.playercount;
            for (int i = 1; i <= message.playercount; i++)
            {
                printf("\tplayer %d at %f %f\n", i, message.playerPositions[i].x, message.playerPositions[i].y);
                model.player[i].x = message.playerPositions[i].x;
                model.player[i].y = message.playerPositions[i].y;
            }
            */
        }
        /*
          for (int i = 0; i < playercount; i++)
        {
            model.player[i].x = message.playerPositions[i].x;
            model.player[i].y = message.playerPositions[i].y;
            updatePlayer(&model.player[i]);
        }
        */

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
