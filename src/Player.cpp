//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

int roundToGridSize(int value, bool up);

struct Point {
    double x;
    double y;
};

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
    return up ? ((value + gridSize - 1) / gridSize) * gridSize : (value / gridSize) * gridSize;
}


Point findNextIntersection(int posX, int posY, double angle) {
    double angleRadians = angle * PI / 180.0;
    double tanTheta = tan(angleRadians);
    double cosTheta = cos(angleRadians);
    double sinTheta = sin(angleRadians);

    // Determine the direction of the ray
    int stepX = cosTheta >= 0 ? 1 : -1;
    int stepY = sinTheta >= 0 ? 1 : -1;

    // Adjust the next grid line calculation
    int nextXGrid = ((posX / GRID_SIZE) + (stepX > 0 ? 1 : (posX % GRID_SIZE == 0 ? 0 : -1))) * GRID_SIZE;
    int nextYGrid = ((posY / GRID_SIZE) + (stepY > 0 ? 1 : (posY % GRID_SIZE == 0 ? 0 : -1))) * GRID_SIZE;

    // Calculate the intersection points
    double intersectionX = nextXGrid;
    double intersectionY = posY + (cosTheta != 0 ? (tanTheta * (intersectionX - posX)) : 0);

    double intersectionYGrid = nextYGrid;
    double intersectionXGrid = posX + (sinTheta != 0 ? ((intersectionYGrid - posY) / tanTheta) : 0);

    // Determine which intersection happens first
    double distX = (cosTheta != 0 ? (intersectionX - posX) / cosTheta : DBL_MAX);
    double distY = (sinTheta != 0 ? (intersectionYGrid - posY) / sinTheta : DBL_MAX);

    Point result;
    if (fabs(distX) < fabs(distY)) {
        result.x = intersectionX;
        result.y = intersectionY;
    } else {
        result.x = intersectionXGrid;
        result.y = intersectionYGrid;
    }

    return result;
}

bool Player::CollisionRayon(int x, int y){
    if(map->getTile(x / (width / nb_case_w),y / (height / nb_case_h)) == 1){
        // cout << x / (width / nb_case_w) << ":" << y / (height / nb_case_h) << endl;
        return true;
    } else{
        return false;
    }
}

void Player::TraceRayon(SDL_Renderer* renderer) {
    double tempX = posX + playerWidth / 2.0;
    double tempY = posY + playerHeight / 2.0;

    bool running = true;
    Point cible = {tempX, tempY};

    while (running) {
        cible = findNextIntersection(tempX, tempY, angle);
        cout << cible.x << endl;
        if (CollisionRayon(cible.x, cible.y)) {
            running = false;
        } else {
            // Check additional grid intersection collision points
            tempX = cible.x;
            tempY = cible.y;
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color red for the ray
    SDL_RenderDrawLine(renderer, posX + playerWidth / 2, posY + playerHeight / 2, cible.x, cible.y);
}