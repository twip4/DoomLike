//
// Created by Paul Baudinot on 25/04/2024.
//

#include "../include/Monster.h"

Monster::Monster(int x, int y, SDL_Texture* texture):  posX(x), posY(y), texture(texture){}

void Monster::Display(SDL_Renderer *renderer, int x, int y) const{
    float distance = sqrt(pow(x - posX, 2) + pow(y - posY, 2)/(width/2500));
    double rectHeight = 50000 / distance;

}
