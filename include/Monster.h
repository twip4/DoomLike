//
// Created by Paul Baudinot on 25/04/2024.
//

#ifndef DOOMLIKE_MONSTER_H
#define DOOMLIKE_MONSTER_H

#include <SDL2/SDL.h>
#include "constante.h"
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include "Map.h"

class Monster {
    public:
    int posX;
    int posY;
    int pv = 100;
    SDL_Texture* texture;
    Monster(int x, int y, SDL_Texture* texture);
    void move(int targetX, int targetY);
};


#endif //DOOMLIKE_MONSTER_H
