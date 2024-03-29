#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PROJECTILE_SIZE 10

typedef struct {
    SDL_Rect rect;
    bool active;
} Projectile;

typedef struct {
    SDL_Rect shipRect;
    SDL_Rect secondShipRect;
    Projectile projectiles[2];
    float shipVelocityX, shipVelocityY;
    float secondShipVelocityX, secondShipVelocityY;
    bool closeWindow;
} Model;

void initializeModel(Model *model);

#endif /* MODEL_H */
