// GameModel.c
#include "GameModel.h"

void initializeModel(GameModel* model) {
    model->x = 300; // Half of WINDOW_WIDTH
    model->y = 200; // Half of WINDOW_HEIGHT
    model->velocityX = model->velocityY = 0;
    model->up = model->down = model->left = model->right = false;
}
