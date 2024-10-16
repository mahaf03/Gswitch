#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "GameModel.h"

#define NETWORK_PORT (49156)
#define SERVERADDRESS "192.168.0.32"
#define STATUS_RESET_PLAYERCOUNT 127 // Placeholder for server network address
// struct defining data sent between client and server
typedef struct udpDataToServer
{
  Player player;
  int status; // idk what this is
} udpDataToServer;
typedef struct PlayerPos
{
  float x;
  float y;
} playerPos;
typedef struct udpDataToClient
{
  Player player;
  int clientId;
  int next30Rand[30];
  int status; // idk what this is
  bool gameReady;
} udpDataToClient;
void initNetwork_Client(UDPsocket *sd, IPaddress *srvadd, UDPpacket *pRecieve);
void initNetwork_Server(UDPsocket *sd);
void closeNetwork_Server(UDPsocket *sd, UDPpacket *pSent);
IPaddress serverReceivePacket(udpDataToServer *packet, UDPsocket *sd);
void serverSendPacket(udpDataToClient Data, IPaddress *destAddr, UDPsocket *sd);
void clientSendPacket(udpDataToServer Data, IPaddress *destAddr, UDPsocket *sd);
IPaddress clientReceivePacket(udpDataToClient *packet, UDPsocket *sd);
float invSqrt(float number);

#endif
