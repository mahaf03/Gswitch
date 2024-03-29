#include "model.h"
#include <SDL2/SDL.h>
#include "../source/defines.h"

void initializeModel(Model *model) {
    model->shipRect.w = model->shipRect.h = model->secondShipRect.w = model->secondShipRect.h = PROJECTILE_SIZE / 4;
    model->shipRect.x = (WINDOW_WIDTH - model->shipRect.w) / 2;
    model->shipRect.y = (WINDOW_HEIGHT - model->shipRect.h) / 2;
    model->secondShipRect = model->shipRect;
    model->secondShipRect.x = (WINDOW_WIDTH - model->secondShipRect.w) / 4; // Start position for the second ship
    model->secondShipRect.y = (WINDOW_HEIGHT - model->secondShipRect.h) / 4;

    for (int i = 0; i < 2; i++) {
        model->projectiles[i].rect.w = model->projectiles[i].rect.h = PROJECTILE_SIZE;
        model->projectiles[i].active = false;
    }

    model->closeWindow = false;
}

