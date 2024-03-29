#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "model.h"

void handleInput(SDL_Event *event, Model *model);

#endif /* CONTROLLER_H */
