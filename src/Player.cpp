//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

Player::Player(int x, int y) : posX(x), posY(y){}

void Player::line_view(SDL_Renderer* renderer) const {
    int stepVue = 0;
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
                float distance = sqrt(pow(x_detect - posX, 2) + pow(y_detect - posY, 2));
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
                SDL_RenderDrawLine(renderer, posX / MiniMap + playerWidth / 2, posY / MiniMap + playerHeight / 2,
                                   x_detect / MiniMap, y_detect / MiniMap);
                int wide = width / NbRayon;
                double rectHeight = 50000 / (cos((angleLine - angle) * M_PI / 180) * distance);  // Correct the distortion formula
                if (rectHeight > height) rectHeight = height;  // Cap the height to the window size

                SDL_Rect rect = {stepVue * wide, static_cast<int>((height - rectHeight) / 2), wide, static_cast<int>(rectHeight)};
                if(
                        (map[(x_detect+1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0 ||
                         map[(x_detect-1)/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==0 ) &&
                        (map[(x_detect)/(width/nb_case_w) + (y_detect+1) /(height/nb_case_h)*nb_case_w]==1 &&
                         map[(x_detect)/(width/nb_case_w) + (y_detect-1) /(height/nb_case_h)*nb_case_w]==1 )
                        ){
                    SDL_SetRenderDrawColor(renderer, 128,128,128, 255);
                }
                else{
                    SDL_SetRenderDrawColor(renderer, 155,155,155, 255);
                }

                SDL_RenderFillRect(renderer, &rect);

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