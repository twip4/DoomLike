//
// Created by Paul Baudinot on 14/04/2024.
//

#ifndef DOOMLIKE_PLAYER_H
#define DOOMLIKE_PLAYER_H
#include "Map.h"
#include "cmath"
#include "constante.h"
class Player {
private:
    bool Collision(int x, int y) const;
public:
    int Posx;
    int Posy;
    int rayon = 1;
    Map* map;
    float angle = 90;
    Player(int x, int y, Map* map);
    void SetPosition(int x, int y);
};

#endif //DOOMLIKE_PLAYER_H
