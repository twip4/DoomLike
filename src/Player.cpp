//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

void DisplayMap(SDL_Renderer* renderer);

Player::Player(int x, int y) : posX(x), posY(y){}

void Player::line_view(SDL_Renderer* renderer) const {
    int stepVue = 0;
    int textureWidth, textureHeight;
    SDL_QueryTexture(wallTexture, NULL, NULL, &textureWidth, &textureHeight);
    DisplayMap(renderer);
    for (float angleLine = angle - fov / 2; angleLine < angle + fov / 2; angleLine += precision_angle) {
        int d_detect = 0;
        bool hit_wall = false;

        while (!hit_wall) {
            int x_detect = posX + playerWidth / 2 + static_cast<int>(cos(angleLine * M_PI / 180) * d_detect);
            int y_detect = posY + playerHeight / 2 - static_cast<int>(sin(angleLine * M_PI / 180) * d_detect);

            if (x_detect < 0 || x_detect >= width || y_detect < 0 || y_detect >= height) {
                hit_wall = true;  // Stop the ray if it goes out of bounds
            } else if (map[x_detect / (width / nb_case_w) + y_detect / (height / nb_case_h) * nb_case_w] == 1) {
                hit_wall = true;
                float distance = sqrt(pow(x_detect - posX, 2) + pow(y_detect - posY, 2)/(width/2500));
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
                SDL_RenderDrawLine(renderer, posX / MiniMap + playerWidth / 2, posY / MiniMap + playerHeight / 2,
                                   x_detect / MiniMap, y_detect / MiniMap);
                int wide = width / NbRayon;
                double rectHeight = 50000 / (cos((angleLine - angle) * M_PI / 180) * distance);
                if (rectHeight > height) rectHeight = height;

                SDL_Rect srcRect;
                SDL_Rect destRect;

                // Optional: Adjust fill color based on adjacency to simulate shading or material difference
                if ((map[(x_detect+1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0 ||
                     map[(x_detect-1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0) &&
                    (map[(x_detect)/(width/nb_case_w) + (y_detect+1) /(height/nb_case_h)*nb_case_w]==1 &&
                     map[(x_detect)/(width/nb_case_w) + (y_detect-1) /(height/nb_case_h)*nb_case_w]==1)) {
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Lighter shade
                    // Calculating the exact texture slice
                    int textureOffset = y_detect % textureWidth;
                    srcRect = {textureOffset, 0, wide, textureHeight};  // Taking a 1-pixel wide slice
                    destRect = {stepVue * wide, static_cast<int>((height - rectHeight) / 2), wide, static_cast<int>(rectHeight)};
                } else {
                    SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255); // Darker shade
                    // Calculating the exact texture slice
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
}

void Player::lineCenter(SDL_Renderer* renderer){
    int x_detect = posX + playerWidth/2 +cos(angle*PI/180)*100;
    int y_detect = posY + playerHeight/2 -sin(angle*PI/180)*100;
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