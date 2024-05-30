//
// Created by Paul Baudinot on 29/05/2024.
//

#ifndef DOOMLIKE_MULTIPLAYER_H
#define DOOMLIKE_MULTIPLAYER_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "../include/Map.h"
#include "../include/constante.h"
#include "../include/Game.h"
#include "../include/Textures.h"
#include "../include/Player.h"
#include "../include/Game.h"

#define PORT 8080

int multi(SDL_Renderer* renderer, TTF_Font* font, Textures T);

#endif //DOOMLIKE_MULTIPLAYER_H
