#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#define NETWORK_PORT (49156)
#define SERVERADDRESS "127.0.0.1" // Placeholder for server network address
//struct defining data sent between client and server
struct udpData{
  int xPos; // players x-coordinate
  int yPos; // players y-coordinate
  int status; //idk what this is
    };
void initNetwork_Client( UDPsocket* sd, IPaddress* srvadd, UDPpacket *p, UDPpacket* pRecieve);
void initNetwork_Server(UDPsocket* sd, UDPpacket* pReceive,UDPpacket* pSent);
void closeNetwork_Server(UDPsocket* sd, UDPpacket* pSent, UDPpacket* pReceive);
int receivePacket(struct udpData* packet, UDPsocket* sd, UDPpacket* pReceive);
void sendPacket(UDPpacket* p, IPaddress* destAddr, UDPsocket* sd);
#endif
