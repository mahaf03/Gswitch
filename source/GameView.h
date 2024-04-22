// GameView.h
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <SDL2/SDL.h>
#include "GameModel.h"

// void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture);
// void renderView(SDL_Renderer* renderer, SDL_Texture* texture, GameModel* model);
// void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture);
void menuView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Texture **volumeTexture);
void renderMenu(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **backgroundTexture, SDL_Texture **continueTexture, SDL_Texture **exitTexture, SDL_Rect continueButtonRect, SDL_Rect exitButtonRect, SDL_Rect volumeButtonRect, SDL_Texture **volumeTexture);
void gameView(SDL_Renderer **renderer, SDL_Window **window, SDL_Texture **texture, SDL_Texture **backgroundTexture, SDL_Texture **blockTexture);
void renderView(SDL_Renderer *renderer, SDL_Texture *shipTexture, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture, GameModel *model, SDL_Rect shipRect);
void closeView(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *shipTexture, SDL_Texture *backgroundTexture, SDL_Texture *blockTexture);
void loadBackground(SDL_Renderer *renderer, SDL_Texture **backgroundTexture);
void loadBlock(SDL_Renderer *renderer, SDL_Texture **blockTexture);
void placeTile(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
void drawLives(SDL_Renderer* renderer, int lives, int x, int y);
void drawExtraLife(SDL_Renderer* renderer, int x, int y);

#endif
