//
// Created by Paul Baudinot on 17/05/2024.
//

#ifndef DOOMLIKE_GAME_H
#define DOOMLIKE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <random>
#include "../include/Map.h"
#include "../include/constante.h"
#include "../include/Game.h"
#include "../include/Textures.h"
#include "../include/Player.h"
#include <cmath>
#include "vector"
#include <thread>

int ModeSolo(SDL_Renderer* renderer, TTF_Font* font, Textures T);

#endif //DOOMLIKE_GAME_H
