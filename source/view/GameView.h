// GameView.h
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <SDL2/SDL.h>
#include "GameModel.h"

void initView(SDL_Renderer** renderer, SDL_Window** window, SDL_Texture** texture);
void renderView(SDL_Renderer* renderer, SDL_Texture* texture, GameModel* model);
void closeView(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture);

#endif
