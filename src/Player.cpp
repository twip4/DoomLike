//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

int roundToGridSize(int value, bool up);

Player::Player(int x, int y, Map* map) : posX(x), posY(y), map(map){}

void Player::SetPosition(int v) {
    float angleRadians = angle * PI / 180.0;
    int proposedX = posX + cos(angleRadians) * v;
    int proposedY = posY + sin(angleRadians) * v;

    switch (Collision(proposedX, proposedY)) {
        case COLLISION_TOP:
            posY = roundToGridSize(proposedY, true);
            break;
        case COLLISION_BOTTOM:
            posY =  roundToGridSize(proposedY + (height / size_map), false)- playerHeight;
            break;
        case COLLISION_RIGHT:
            posX = roundToGridSize(proposedX + (width / size_map), true) - playerWidth - (width / size_map);
            break;
        case COLLISION_LEFT:
            posX = roundToGridSize(proposedX, true);
            break;
        case COLLISION_OK:
            posX = proposedX;
            posY = proposedY;
            break;
        case COLLISION_OUTER:
            // Gérer le cas où le joueur est à l'extérieur des limites autorisées
            break;
        default:
            cout << "Error in collision detection" << endl;
    }
}



CollisionResult Player::Collision(int x, int y) const {
    // cout << x << ":" << y << endl;

    // Vérification préliminaire pour s'assurer que le joueur reste dans les limites
    if (x < 0 || y < 0 || (x + playerWidth > width) || (y + playerHeight > height)) {
        return COLLISION_OUTER;
    }

    // Calcul des indices des coins dans la grille de la carte
    int left = x / (width / nb_case_w);
    int right = (x + playerWidth - 1) / (width / nb_case_w);
    int top = y / (height / nb_case_h);
    int bottom = (y + playerHeight - 1) / (height / nb_case_h);

    if ((map->getTile(left, top) == 1 && map->getTile(right, top) == 1)){
        return COLLISION_TOP;
    }

    if ((map->getTile(left, top) == 1 && map->getTile(left, bottom) == 1)){
        return COLLISION_LEFT;
    }


    if ((map->getTile(left, bottom) == 1 && map->getTile(right, bottom) == 1)){
        return COLLISION_BOTTOM;
    }

    if ((map->getTile(right, bottom) == 1 && map->getTile(right, top) == 1)){
        return COLLISION_RIGHT;
    }

    return COLLISION_OK;
}

int roundToGridSize(int value, bool up) {
    int gridSize = height / size_map;
    if (up) {
        return ((value + gridSize - 1) / gridSize) * gridSize; // Arrondi supérieur
    } else {
        return (value / gridSize) * gridSize; // Arrondi inférieur
    }
}

void Player::calculateDescentRatio() {
    float angleRadians = angle * PI / 180.0;
    double m = tan(angleRadians);
    int lastValidX = posX;
    int lastValidY = posY;

    int tempX = posX;
    int tempY = posY;

    while (true) {
        if (std::abs(cos(angleRadians)) > std::abs(sin(angleRadians))) {
            tempX = roundToGridSize(tempX + (cos(angleRadians) > 0 ? 1 : -1), cos(angleRadians) > 0);
            tempY = posY + m * (tempX - posX);
        } else {
            tempY = roundToGridSize(tempY + (sin(angleRadians) > 0 ? 1 : -1), sin(angleRadians) > 0);
            tempX = posX + (tempY - posY) / m;
        }

        if (Collision(tempX, tempY) != COLLISION_OK) {
            break;
        }

        lastValidX = tempX;  // Mise à jour de la dernière position valide
        lastValidY = tempY;
    }

    std::cout << "Last valid position was X: " << lastValidX << " Y: " << lastValidY << std::endl;
}

void Player::TraceRayon(SDL_Renderer* renderer){
    // Conversion de l'angle de degrés en radians pour les fonctions trigonométriques
    float angleRadians = angle * PI / 180.0;

    double m = tan(angleRadians);

    int tempX = posX;
    int tempY = posY;

    while (true) {
        if (std::abs(cos(angleRadians)) > std::abs(sin(angleRadians))) {
            tempX = roundToGridSize(tempX + (cos(angleRadians) > 0 ? 1 : -1), cos(angleRadians) > 0);
            tempY = posY + m * (tempX - posX);
        } else {
            tempY = roundToGridSize(tempY + (sin(angleRadians) > 0 ? 1 : -1), sin(angleRadians) > 0);
            tempX = posX + (tempY - posY) / m;
        }

        if (Collision(tempX, tempY) != COLLISION_OK) {
            break;
        }
    }

    // Calcul des coordonnées finales en utilisant la longueur du rayon
    int endX = tempX;
    int endY = tempY;

    // Définir la couleur de dessin pour le rayon
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le rayon

    // Tracer la ligne du centre du joueur vers le point calculé
    SDL_RenderDrawLine(renderer, posX + playerWidth / 2, posY + playerHeight / 2, endX, endY);
}