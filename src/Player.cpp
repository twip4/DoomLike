//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

void DisplayMap(SDL_Renderer* renderer);
Player::Player(int x, int y,std::vector<Monster>* listMonster) : posX(x), posY(y), listMonster(listMonster){}

void Player::line_view(SDL_Renderer* renderer) const {
    int stepVue = 0;
    int textureWidth, textureHeight;
    SDL_QueryTexture(wallTexture, NULL, NULL, &textureWidth, &textureHeight);
    DisplayMap(renderer);
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
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
                SDL_RenderDrawLine(renderer, (posX+ playerWidth / 2 )/ MiniMap,  (posY+ playerWidth / 2 ) / MiniMap,
                                   x_detect / MiniMap, y_detect / MiniMap);
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

                    SDL_SetTextureColorMod(wallTexture, 168, 168, 168);
                    int textureOffset = y_detect % textureWidth;
                    srcRect = {textureOffset, 0, wide, textureHeight};  // Taking a 1-pixel wide slice
                    destRect = {stepVue * wide, static_cast<int>((height - rectHeight) / 2), wide, static_cast<int>(rectHeight)};
                } else {
                    SDL_SetTextureColorMod(wallTexture, 128, 128, 128);
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
}

void Player::lineCenter(SDL_Renderer* renderer){
    int x_detect = posX + playerWidth/2 +cos(angle*PI/180)*100;
    int y_detect = posY + playerHeight/2 +sin(angle*PI/180)*100;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set the color to red for the ray
    SDL_RenderDrawLine(renderer, posX / MiniMap + playerWidth / 2 , posY /MiniMap  + playerHeight / 2 , x_detect / MiniMap, y_detect / MiniMap);
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


// Fonction pour calculer le produit vectoriel entre deux vecteurs
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
    // Assume that posX, posY, playerWidth, and playerHeight are defined and valid.
    Point playerCenter = {static_cast<double>(posX + playerWidth / 2), static_cast<double>(posY + playerHeight / 2)};
    for (const auto& monster : *listMonster) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(monster.texture, NULL, NULL, &textureWidth, &textureHeight);

        // Calculate the angle to the monster from player's perspective
        double angleMonster = atan2(monster.posY - playerCenter.y, monster.posX - playerCenter.x) * 180 / M_PI;
        // Normalize angleMonster to be within [0, 360]
        if (angleMonster < 0) angleMonster += 360;

        // Only process this monster if it's within the visible arc
        if (angleMonster >= angleStart && angleMonster <= angleStop) {
            // Segment the monster texture and display each part
            for (int i = 0; i < NbMonsterSplit; ++i) {
                // Calculate texture segments
                int segmentWidth = textureWidth / NbMonsterSplit;
                SDL_Rect srcRect = {
                        i * segmentWidth, 0,
                        segmentWidth, textureHeight
                };

                int d_detect = 0;
                bool hit_wall = false;
                int x_detect;
                int y_detect;

                while (!hit_wall) {
                    x_detect = (posX + playerWidth / 2) + static_cast<int>(cos(angleMonster * M_PI / 180) * d_detect);
                    y_detect = (posY + playerHeight / 2) + static_cast<int>(sin(angleMonster * M_PI / 180) * d_detect);

                    if (x_detect < 0 || x_detect >= width || y_detect < 0 || y_detect >= height) {
                        hit_wall = true;  // Stop the ray if it goes out of bounds
                    } else if (map[x_detect / (width / nb_case_w) + y_detect / (height / nb_case_h) * nb_case_w] == 1) {
                        hit_wall = true;
                    }
                    d_detect++;
                }

                // Distance and size adjustments for perspective
                float distanceToMonster = sqrt(pow(monster.posX - playerCenter.x, 2) + pow(monster.posY - playerCenter.y, 2));
                float distanceToWall = sqrt(pow(x_detect - playerCenter.x, 2) + pow(y_detect - playerCenter.y, 2));

                if (distanceToMonster<distanceToWall){
                    double rectHeight = 50000 / distanceToMonster;  // Simplified perspective scaling
                    rectHeight = std::min(rectHeight, static_cast<double>(height));  // Clamp height

                    // Determine where to draw this segment on screen
                    double segmentScreenX = ((angleMonster - angleStart) / (angleStop - angleStart)) * width;
                    SDL_Rect destRect = {
                            static_cast<int>(segmentScreenX + i * (segmentWidth * rectHeight / textureHeight))-10,
                            static_cast<int>((height / 2) - (rectHeight / 2)),
                            static_cast<int>(segmentWidth * rectHeight / textureHeight),
                            static_cast<int>(rectHeight)
                    };

                    SDL_RenderCopy(renderer, monster.texture, &srcRect, &destRect);
                }
            }
        }
    }
}
