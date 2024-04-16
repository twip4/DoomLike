//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

Player::Player(int x, int y, Map* map) : Posx(x), Posy(y), map(map){}

void Player::SetPosition(int x, int y) {
    if(Collision(Posx + x, Posy + y)){
        Posx += x;
        Posy += y;
    }
}

bool Player::Collision(int x, int y) const {
    //cout << x << ":" << y << endl;

    int playerWidth = (width / nb_case_w) / rapportPlayerMaps;
    int playerHeight = (height / nb_case_h) / rapportPlayerMaps;

    if ((x >= 0) && (y >= 0) && ((x + playerWidth - 1) <= width) && ((y + playerHeight - 1) <= height)) {
        // Vérifie chaque coin du joueur pour les collisions
        if (map->getTile(x / (width / nb_case_w), y / (height / nb_case_h)) == 1 ||
            map->getTile((x + playerWidth - 1) / (width / nb_case_w), y / (height / nb_case_h)) == 1 ||
            map->getTile(x / (width / nb_case_w), (y + playerHeight - 1) / (height / nb_case_h)) == 1 ||
            map->getTile((x + playerWidth - 1) / (width / nb_case_w), (y + playerHeight - 1) / (height / nb_case_h)) == 1) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}