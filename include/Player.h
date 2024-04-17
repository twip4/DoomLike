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

enum CollisionResult {
    COLLISION_TOP,
    COLLISION_BOTTOM,
    COLLISION_RIGHT,
    COLLISION_LEFT,
    COLLISION_OK,
    COLLISION_OUTER,
    COLLISION_ERROR
};

class Player {
private:
    CollisionResult Collision(int x, int y) const;
public:
    int posX;
    int posY;
    int rayon = 1;
    Map* map;
    int angle = 90;
    Player(int x, int y, Map* map);
    void TraceRayon(SDL_Renderer* renderer);
    void SetPosition(int v);
    bool CollisionRayon(int x, int y);
};

#endif //DOOMLIKE_PLAYER_H
