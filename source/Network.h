#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#define NETWORK_PORT (49156);
//struct defining data sent between client and server
struct udpData{
  int xPos; // players x-coordinate
  int yPos; // players y-coordinate
  int status; //idk what this is
    };
void initNetwork();


#endif
