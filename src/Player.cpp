//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

void DisplayMap(SDL_Renderer* renderer);

Player::Player(int x, int y,std::vector<Monster*>* listMonster) : posX(x), posY(y), listMonster(listMonster){}

void Player::view(SDL_Renderer* renderer) const {
    int stepVue = 0;
    int textureWidth, textureHeight;
    SDL_QueryTexture(wallTexture, NULL, NULL, &textureWidth, &textureHeight);
    float angleStart = angle - fov / 2;
    float angleEnd = angle + fov / 2;

    for (float angleLine = (angleStart); angleLine < (angleEnd); angleLine += precision_angle) {


        int d_detect = 0;
        bool hit_wall = false;

        while (!hit_wall) {
            int x_detect = (posX + playerWidth / 2) + static_cast<int>(cos(angleLine * M_PI / 180) * d_detect);
            int y_detect = (posY + playerHeight / 2) + static_cast<int>(sin(angleLine * M_PI / 180) * d_detect);

            if (x_detect < 0 || x_detect >= width || y_detect < 0 || y_detect >= height) {
                hit_wall = true;  // Stop the ray if it goes out of bounds
            } else if (map[x_detect / (width / nb_case_w) + y_detect / (height / nb_case_h) * nb_case_w] == 1) {
                hit_wall = true;
                float distance = sqrt(pow(x_detect - posX, 2) + pow(y_detect - posY, 2));
                // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
                // SDL_RenderDrawLine(renderer, (posX+ playerWidth / 2 )/ MiniMap,  (posY+ playerWidth / 2 ) / MiniMap, x_detect / MiniMap, y_detect / MiniMap);
                int wide = 1;
                double rectHeight = 50000 / (cos((angleLine - angle) * M_PI / 180) * distance);
                if (rectHeight > height) rectHeight = height;

                SDL_Rect srcRect;
                SDL_Rect destRect;

                // Optional: Adjust fill color based on adjacency to simulate shading or material difference
                if ((map[(x_detect+1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0 ||
                     map[(x_detect-1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0) &&
                    (map[(x_detect)/(width/nb_case_w) + (y_detect+1) /(height/nb_case_h)*nb_case_w]==1 &&
                     map[(x_detect)/(width/nb_case_w) + (y_detect-1) /(height/nb_case_h)*nb_case_w]==1)) {

                    SDL_SetTextureColorMod(wallTexture, 255, 255, 255);
                    int textureOffset = y_detect % textureWidth;
                    srcRect = {textureOffset, 0, wide, textureHeight};  // Taking a 1-pixel wide slice
                    destRect = {stepVue * wide, static_cast<int>((height - rectHeight) / 2), wide, static_cast<int>(rectHeight)};
                } else {
                    SDL_SetTextureColorMod(wallTexture, 200, 200, 200);
                    int textureOffset = x_detect % textureWidth;
                    srcRect = {textureOffset, 0, wide, textureHeight};  // Taking a 1-pixel wide slice
                    destRect = {stepVue * wide, static_cast<int>((height - rectHeight) / 2), wide, static_cast<int>(rectHeight)};
                }

                SDL_RenderFillRect(renderer, &destRect); // Fill the rectangle first to provide a background color
                SDL_RenderCopy(renderer, wallTexture, &srcRect, &destRect); // Render the texture slice over it
            }
            d_detect++;
        }
        stepVue++;
    }
    DisplayMonster(angleStart,angleEnd, renderer);
    DisplayMap(renderer);
}

void Player::lineCenter(SDL_Renderer* renderer){
    int x_detect = posX + playerWidth/2 +cos(angle*PI/180)*100;
    int y_detect = posY + playerHeight/2 +sin(angle*PI/180)*100;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Set the color to red for the ray
    SDL_RenderDrawLine(renderer, posX / MiniMap  , posY /MiniMap , x_detect / MiniMap, y_detect / MiniMap);
}

void DisplayMap(SDL_Renderer* renderer){
    for(int i=0;i<nb_case_w*nb_case_h;i++){
        SDL_Rect rect;
        rect.x = i%nb_case_w*width/nb_case_w/MiniMap;
        rect.y = floor(i/nb_case_h)*height/nb_case_h/MiniMap;
        rect.w = width/size_map/MiniMap+1;
        rect.h = height/size_map/MiniMap+1;
        if(map[i]==1){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

double crossProduct(const Point& A, const Point& B) {
    return A.x * B.y - A.y * B.x;
}

bool isInsideTriangle(const Point& A, const Point& B, const Point& C, const Point& P) {
    // Calculez les vecteurs formés par les côtés du triangle
    Point AB = {B.x - A.x, B.y - A.y};
    Point BC = {C.x - B.x, C.y - B.y};
    Point CA = {A.x - C.x, A.y - C.y};

    // Calculez les vecteurs entre le point donné et chaque sommet du triangle
    Point AP = {P.x - A.x, P.y - A.y};
    Point BP = {P.x - B.x, P.y - B.y};
    Point CP = {P.x - C.x, P.y - C.y};

    // Calculez les produits vectoriels entre chaque côté du triangle et les vecteurs AP, BP, CP
    double crossABP = crossProduct(AB, AP);
    double crossBCP = crossProduct(BC, BP);
    double crossCAP = crossProduct(CA, CP);

    // Vérifiez si les trois produits vectoriels ont le même signe
    return (crossABP > 0 && crossBCP > 0 && crossCAP > 0) ||
           (crossABP < 0 && crossBCP < 0 && crossCAP < 0);
}

void Player::DisplayMonster(float angleStart, float angleStop, SDL_Renderer* renderer) const {
    Point pos = {(double) posX, (double) posY};
    Point zoneStart = {posX + cos(angleStart * M_PI / 180) * width, posY + sin(angleStart * M_PI / 180) * width};
    Point zoneStop = {posX + cos(angleStop * M_PI / 180) * width, posY + sin(angleStop * M_PI / 180) * width};

    std::vector<std::pair<Monster*, double>> listMonsterSort;

    // Calculer les distances et remplir le vecteur temporaire
    for (const auto& monster : *listMonster) {
        double distance = sqrt(pow(monster->posX - posX, 2) + pow(monster->posY - posY, 2));
        listMonsterSort.emplace_back(monster, distance);
    }

    // Trier les monstres par distance croissante
    std::sort(listMonsterSort.begin(), listMonsterSort.end(), [](const std::pair<Monster*, double>& a, const std::pair<Monster*, double>& b) {
        return a.second > b.second;
    });


    for (const auto& pair : listMonsterSort) {
        Monster* monster = pair.first;
        Point posMonster = {(double) monster->posX, (double) monster->posY};
        int textureWidth, textureHeight;
        SDL_QueryTexture(monster->texture, NULL, NULL, &textureWidth, &textureHeight);


        if (isInsideTriangle(pos,zoneStart,zoneStop,posMonster)) {

            double angleMonstre = atan2( monster->posY - (posY + playerHeight/2), monster->posX - (posX + playerWidth/2));

            // std::cout << angle << ":" << angleMonstre / M_PI * 180 << std::endl;

            int d_detect = 0;
            bool hit_wall = false;

            int x_detect;
            int y_detect;

            while (!hit_wall) {

                x_detect = posX + playerWidth / 2 + static_cast<int>(cos(angleMonstre) * d_detect);
                y_detect = posY + playerHeight / 2 + static_cast<int>(sin(angleMonstre) * d_detect);

                if (x_detect < 0 || x_detect >= width || y_detect < 0 || y_detect >= height) {
                    hit_wall = true;  // Stop the ray if it goes out of bounds
                } else if (map[x_detect / (width / nb_case_w) + y_detect / (height / nb_case_h) * nb_case_w] == 1) {
                    hit_wall = true;
                }
                d_detect++;
            }

            float distancePoint = sqrt(pow(x_detect - posX, 2) + pow(y_detect - posY, 2));
            float distanceMonster = sqrt(pow(monster->posX - posX, 2) + pow(monster->posY - posY, 2));

            // std::cout << distancePoint << ":" << distanceMonster << std::endl;

            if (distancePoint >= distanceMonster){
                float distance = sqrt(pow(monster->posX - posX, 2) + pow(monster->posY - posY, 2));
                double rectHeight = 50000 / distance;
                rectHeight = std::min(rectHeight, static_cast<double>(height));
                int rectWidth = static_cast<int>(textureWidth / (textureHeight / rectHeight));
                float rationX =  (float)((angleMonstre*180 / M_PI)-angleStart)/(float)(fov) ;

                if (rationX < 0){
                    rationX += 6;
                }

                SDL_Rect srcRect = {0, 0, textureWidth, textureHeight};
                SDL_Rect destRect = {static_cast<int>(width*rationX - (rectWidth/2)),
                                     static_cast<int>((height / 2) - (rectHeight / 2)),
                                     rectWidth,
                                     static_cast<int>(rectHeight)};

                SDL_SetTextureColorMod(monster->texture, 150, 150, 150);

                SDL_RenderCopy(renderer, monster->texture, &srcRect, &destRect);
            }


        }
    }
}

void Player::shot() {
    Point pos = {(double) posX, (double) posY};
    Point zoneStart = {posX + cos((angle - fov / 2) * M_PI / 180) * width,
                       posY + sin((angle - fov / 2) * M_PI / 180) * width};
    Point zoneStop = {posX + cos((angle + fov / 2) * M_PI / 180) * width,
                      posY + sin((angle + fov / 2) * M_PI / 180) * width};

    for (auto it = listMonster->begin(); it != listMonster->end();) {
        Monster* monster = *it;
        Point posMonster = {static_cast<double>(monster->posX), static_cast<double>(monster->posY)};

        if (isInsideTriangle(pos, zoneStart, zoneStop, posMonster)) {
            float distanceMonster = sqrt(pow(monster->posX - posX, 2) + pow(monster->posY - posY, 2));
            float x = posX + playerWidth / 2 + static_cast<int>(cos(angle * M_PI / 180) * distanceMonster);
            float y = posY + playerHeight / 2 + static_cast<int>(sin(angle * M_PI / 180) * distanceMonster);

            bool hit_wall = false;
            int d_detect = 1;
            int x_detect;
            int y_detect;

            while (!hit_wall && d_detect <= distanceMonster) {
                x_detect = posX + playerWidth / 2 + static_cast<int>(cos(angle) * d_detect);
                y_detect = posY + playerHeight / 2 + static_cast<int>(sin(angle) * d_detect);

                if (x_detect < 0 || x_detect >= width || y_detect < 0 || y_detect >= height) {
                    hit_wall = true;  // Stop the ray if it goes out of bounds
                } else if (map[x_detect / (width / nb_case_w) + y_detect / (height / nb_case_h) * nb_case_w] == 1) {
                    hit_wall = true;
                }
                d_detect++;
            }

            float distanceShot = sqrt(pow(x - monster->posX, 2) + pow(y - monster->posY, 2));

            if (distanceShot <= tolerance) {
                monster->pv -= degas;
                if (monster->pv <= 0) {
                    score += 100;
                    it = listMonster->erase(it);  // Erase and get the next iterator
                    continue;  // Skip the increment to prevent skipping the next element
                }
            }
        }
        ++it;  // Move to the next element if not erased
    }
}