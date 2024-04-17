//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

struct Point {
    int x;
    int y;
};

Player::Player(int x, int y, Map* map) : posX(x), posY(y), map(map){}


int roundToGridSize(int value, bool up) {
    int gridSize = height / size_map;
    return up ? ((value + gridSize - 1) / gridSize) * gridSize : (value / gridSize) * gridSize;
}


void Player::SetPosition(int v) {
    float angleRadians = angle * PI / 180.0;
    int proposedX = posX + cos(angleRadians) * v;
    int proposedY = posY + sin(angleRadians) * v;

    switch (Collision(proposedX, proposedY)) {
        case COLLISION_TOP:
            posY = roundToGridSize(proposedY, true);
            posX = proposedX;
            break;
        case COLLISION_BOTTOM:
            posY =  roundToGridSize(proposedY + (height / size_map), false)- playerHeight;
            posX = proposedX;
            break;
        case COLLISION_RIGHT:
            posX = roundToGridSize(proposedX + (width / size_map), true) - playerWidth - (width / size_map);
            posY = proposedY;
            break;
        case COLLISION_LEFT:
            posX = roundToGridSize(proposedX, true);
            posY = proposedY;
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

Point next_grid_intersection(double x, double y, double theta) {
    // Convertir l'angle en radians
    double theta_radians = theta * M_PI / 180.0;

    // Calculer les composantes du vecteur direction
    double dx = std::cos(theta_radians);
    double dy = std::sin(theta_radians);

    // Trouver le prochain multiple de SIZE_GRID pour x et y
    double next_x, next_y;
    if (dx > 0) {
        next_x = std::ceil(x / GRID_SIZE) * GRID_SIZE;
    } else if (dx < 0) {
        next_x = std::floor(x / GRID_SIZE) * GRID_SIZE;
    } else {
        next_x = x;
    }

    if (dy > 0) {
        next_y = std::ceil(y / GRID_SIZE) * GRID_SIZE;
    } else if (dy < 0) {
        next_y = std::floor(y / GRID_SIZE) * GRID_SIZE;
    } else {
        next_y = y;
    }

    // Calculer la distance jusqu'aux prochaines intersections
    double dist_x = dx != 0 ? (next_x - x) / dx : std::numeric_limits<double>::infinity();
    double dist_y = dy != 0 ? (next_y - y) / dy : std::numeric_limits<double>::infinity();

    Point cible{};
    // Comparer les distances et choisir la plus courte
    if (std::abs(dist_x) < std::abs(dist_y)) {
        return cible = {static_cast<int>(next_x), static_cast<int>(y + dy * dist_x)};
    } else {
        return cible = {static_cast<int>(x + dx * dist_y), static_cast<int>(next_y)};
    }
}


bool Player::CollisionRayon(int startX, int startY, int x, int y) {
    int tileX = x / (width / nb_case_w);
    int tileY = y / (height / nb_case_h);

    if (x % GRID_SIZE == 0 && y % GRID_SIZE == 0){
        if (startY < y) {
            if (startX < x) {
                return map->getTile(tileX + 1, tileY + 1) == 1;
            } else {
                return map->getTile(tileX - 1, tileY + 1) == 1;
            }
        } else {
            if (startX < x) {
                return map->getTile(tileX + 1, tileY - 1) == 1;
            } else {
                return map->getTile(tileX - 1, tileY- 1) == 1;
            }
        }
    }
    if (x % GRID_SIZE == 0) {
        if (startX < x) {
            return map->getTile(tileX, tileY) == 1;
        } else {
            return map->getTile(tileX - 1, tileY) == 1;
        }
    }
    if (y % GRID_SIZE == 0) {
        if (startY < y) {
            return map->getTile(tileX, tileY) == 1;
        } else {
            return map->getTile(tileX, tileY - 1) == 1;
        }
    }
}

void Player::TraceRayon(SDL_Renderer* renderer) {
    for (int i = 0; i < fov; i += precision_angle){
        double tempX = posX + playerWidth / 2.0;
        double tempY = posY + playerHeight / 2.0;

        bool running = true;
        Point cible;

        int step = 0;

        while (running) {
            cible = next_grid_intersection(tempX, tempY, angle+i);
            if (CollisionRayon(tempX, tempY, cible.x, cible.y)) {
                running = false;
            } else {
                if (tempX < cible.x) {
                    tempX = cible.x+1;
                } else{
                    tempX = cible.x-1;
                }
                if (tempY < cible.y) {
                    tempY = cible.y+1;
                } else{
                    tempY = cible.y-1;
                }
            }
            if (step >= size_map){
                running = false;
            }
            step ++;
        }

        float distance = sqrt(pow(cible.x - posX + playerWidth / 2,2)+pow(cible.y - posY + playerHeight / 2,2));
        float rectHeight = 50000/(cos((fov / 2 - i/precision_angle)*PI/180)*distance);
        float wide = (float) width / (fov*precision_angle);
        float rectWidth = wide;

        SDL_Rect rect;
        rect.x = i*wide;
        rect.y = (height -rectHeight )/2;
        rect.w = rectWidth;
        rect.h = rectHeight;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
        //SDL_RenderDrawLine(renderer, posX + playerWidth / 2, posY + playerHeight / 2, cible.x, cible.y);
    }
}