//
// Created by Paul Baudinot on 05/05/2024.
//

#include "../include/Textures.h"

SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer);
SDL_Texture* loadBMPTexureTransparant(const char* filepath, SDL_Renderer* renderer);

Textures::Textures(SDL_Renderer *renderer) {
    wallTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/Wall1.bmp", renderer);
    if (!wallTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

    groundTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/ground.bmp", renderer);
    if (!groundTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

    skyTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/sky.bmp", renderer);
    if (!skyTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

    monsterTexture = loadBMPTexureTransparant("/Users/paulbaudinot/CLionProjects/DoomLike/assets/monster.bmp",renderer);

    HUDTexture = loadBMPTexureTransparant("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD.bmp",renderer);

    HUDSHOT1Texture = loadBMPTexureTransparant("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD_SHOT1.bmp",renderer);

    HUDSHOT2Texture = loadBMPTexureTransparant("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD_SHOT2.bmp",renderer);

    HUDSHOT3Texture = loadBMPTexureTransparant("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD_SHOT3.bmp",renderer);
}

SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer) {
    SDL_Surface* bmpSurface = SDL_LoadBMP(filepath);
    if (!bmpSurface) {
        SDL_Log("Unable to load BMP file: %s", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
    SDL_FreeSurface(bmpSurface);  // Libérez la surface après la création de la texture

    if (!texture) {
        SDL_Log("Unable to create texture from BMP file: %s", SDL_GetError());
    }

    return texture;
}

SDL_Texture* loadBMPTexureTransparant(const char* filepath, SDL_Renderer* renderer){
    SDL_Surface* Surface = IMG_Load(filepath);
    if (!Surface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

// Set color key for transparency if needed
    SDL_SetColorKey(Surface, SDL_TRUE, SDL_MapRGB(Surface->format, 0, 0xFF, 0xFF));

// Create texture from surface
    SDL_Texture* Texture = SDL_CreateTextureFromSurface(renderer, Surface);
    if (!Texture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(Surface); // Free the surface before returning
    }

// Free the surface after creating the texture
    SDL_FreeSurface(Surface);

    SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND);

    return Texture;
};