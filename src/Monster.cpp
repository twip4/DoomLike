//
// Created by Paul Baudinot on 25/04/2024.
//

#include "../include/Monster.h"

Monster::Monster(int x, int y, SDL_Texture* texture):  posX(x), posY(y), texture(texture){}

void Monster::move(int targetX, int targetY) {
        posX += targetX;
        posY += targetY;
}

Monster::~Monster() {
    // Libérer les ressources allouées
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}