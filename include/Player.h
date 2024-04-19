//
// Created by Paul Baudinot on 14/04/2024.
//

#ifndef DOOMLIKE_PLAYER_H
#define DOOMLIKE_PLAYER_H
#include <SDL2/SDL.h>
#include "Map.h"
#include "cmath"
#include <algorithm>
#include "constante.h"

struct Point {
    int x;
    int y;
};

class Player {
public:
    int posX;
    int posY;
    int angle = 90;
    Player(int x, int y);
    void line_view(SDL_Renderer* renderer, float nb_dec) const;
    void lineCenter(SDL_Renderer* renderer);
};

#endif //DOOMLIKE_PLAYER_H
