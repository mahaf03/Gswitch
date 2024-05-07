// GameController.h
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SDL2/SDL.h>
#include "GameModel.h"

void handleEvent(SDL_Event* event, Player* player, bool* closeWindow);
void updatePlayer(Player* player);
void stopPlayer(Player* player, int direction);

#endif
