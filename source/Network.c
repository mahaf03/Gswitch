#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include "Network.h"

void initNetwork( UDPsocket* sd, IPaddress* srvadd, UDPpacket **p, UDPpacket** pRecieve)
  {
   if (SDLNet_Init() <0)
     {
       fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
       exit(EXIT_FAILURE);
     }

  }
