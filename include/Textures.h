//
// Created by Paul Baudinot on 05/05/2024.
//

#ifndef DOOMLIKE_TEXTURES_H
#define DOOMLIKE_TEXTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Textures{
public:
    Textures(SDL_Renderer* renderer);
    SDL_Texture* wallTexture;
    SDL_Texture* groundTexture;
    SDL_Texture* skyTexture;
    SDL_Texture* monsterTexture;
    SDL_Texture* HUDTexture;
    SDL_Texture* HUDSHOT1Texture;
    SDL_Texture* HUDSHOT2Texture;
    SDL_Texture* HUDSHOT3Texture;
};

#endif //DOOMLIKE_TEXTURES_H
