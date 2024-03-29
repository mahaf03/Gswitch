#ifndef VIEW_H
#define VIEW_H

#include "model.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void render(SDL_Renderer *renderer, SDL_Texture *texture, Model *model);

#endif /* VIEW_H */
