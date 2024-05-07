//
// Created by Paul Baudinot on 05/05/2024.
//

#include "../include/Textures.h"

SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer);

Textures::Textures(SDL_Renderer *renderer) {
    wallTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/Wall.bmp", renderer);
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

    SDL_Surface* monsterSurface = IMG_Load("/Users/paulbaudinot/CLionProjects/DoomLike/assets/monster.bmp");
    if (!monsterSurface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

// Set color key for transparency if needed
    SDL_SetColorKey(monsterSurface, SDL_TRUE, SDL_MapRGB(monsterSurface->format, 0, 0xFF, 0xFF));

// Create texture from surface
    monsterTexture = SDL_CreateTextureFromSurface(renderer, monsterSurface);
    if (!monsterTexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(monsterSurface); // Free the surface before returning
    }

// Free the surface after creating the texture
    SDL_FreeSurface(monsterSurface);

    SDL_SetTextureBlendMode(monsterTexture, SDL_BLENDMODE_BLEND);


    SDL_Surface* HUDSurface = IMG_Load("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD_FPS.bmp");
    if (!HUDSurface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

// Set color key for transparency if needed
    SDL_SetColorKey(HUDSurface, SDL_TRUE, SDL_MapRGB(HUDSurface->format, 0, 0xFF, 0xFF));

// Create texture from surface
    HUDTexture = SDL_CreateTextureFromSurface(renderer, HUDSurface);
    if (!HUDTexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(HUDSurface); // Free the surface before returning
    }

// Free the surface after creating the texture
    SDL_FreeSurface(HUDSurface);

    SDL_SetTextureBlendMode(HUDTexture, SDL_BLENDMODE_BLEND);


    SDL_Surface* HUD_FIRESurface = IMG_Load("/Users/paulbaudinot/CLionProjects/DoomLike/assets/HUD_FPS_FIRE.bmp");
    if (!HUD_FIRESurface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

// Set color key for transparency if needed
    SDL_SetColorKey(HUD_FIRESurface, SDL_TRUE, SDL_MapRGB(HUD_FIRESurface->format, 0, 0xFF, 0xFF));

// Create texture from surface
    HUD_FIRETexture = SDL_CreateTextureFromSurface(renderer, HUD_FIRESurface);
    if (!HUD_FIRETexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(HUD_FIRESurface); // Free the surface before returning
    }

// Free the surface after creating the texture
    SDL_FreeSurface(HUD_FIRESurface);

    SDL_SetTextureBlendMode(HUD_FIRETexture, SDL_BLENDMODE_BLEND);

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