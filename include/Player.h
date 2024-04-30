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
#include "Monster.h"
#include <SDL2/SDL2_gfxPrimitives.h>
struct Point {
    double x, y;
};

class Player {
private:
    void DisplayMonster(float angleStart, float angleStop, SDL_Renderer* renderer) const;
public:
    int posX;
    int posY;
    int angle = 0;
    SDL_Texture* wallTexture;
    std::vector<Monster>* listMonster;
    Player(int x, int y,std::vector<Monster>* listMonster);
    void line_view(SDL_Renderer* renderer) const;
    void lineCenter(SDL_Renderer* renderer);
};

#endif //DOOMLIKE_PLAYER_H
