// GameModel.h
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <stdbool.h>

typedef struct {
    float x, y; // Player's position
    float velocityX, velocityY; // Player's velocity
    bool up, down, left, right; // Direction flags
    bool collisionUp, collisionDown, collisionLeft, collisionRight; // Collision flags
} GameModel;

void initializeModel(GameModel* model);

#endif
