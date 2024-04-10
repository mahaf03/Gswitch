// GameController.h
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SDL2/SDL.h>
#include "GameModel.h"

void handleEvent(SDL_Event* event, GameModel* model, bool* closeWindow);
void updateModel(GameModel* model);
void stopModel(GameModel* model, int collision);

#endif
