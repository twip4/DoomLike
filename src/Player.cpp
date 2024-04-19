//
// Created by Paul Baudinot on 14/04/2024.
//

#include "../include/Player.h"

Player::Player(int x, int y) : posX(x), posY(y){}

void Player::line_view(SDL_Renderer* renderer, float angleLine) const{
    int d_detect=0;
    bool hit_wall = false;

    while (!hit_wall){
        int x_detect = posX + playerWidth/2 +cos(angleLine*PI/180)*d_detect;
        int y_detect = posY + playerHeight/2 -sin(angleLine*PI/180)*d_detect;
        if(map[x_detect/(width/nb_case_w) + y_detect /(height/nb_case_h)*nb_case_w]==1){
            hit_wall = true;
            float distance = sqrt(pow(x_detect - posX,2)+pow(y_detect - posY,2));
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set the color to red for the ray
            SDL_RenderDrawLine(renderer, posX / MiniMap + playerWidth / 2 , posY /MiniMap  + playerHeight / 2 , x_detect / MiniMap, y_detect / MiniMap);
        }
        d_detect++;
    }
}

void Player::lineCenter(SDL_Renderer* renderer){
    int x_detect = posX + playerWidth/2 +cos(angle*PI/180)*100;
    int y_detect = posY + playerHeight/2 -sin(angle*PI/180)*100;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set the color to red for the ray
    SDL_RenderDrawLine(renderer, posX / MiniMap + playerWidth / 2 , posY /MiniMap  + playerHeight / 2 , x_detect / MiniMap, y_detect / MiniMap);
}