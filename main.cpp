#include <SDL2/SDL.h>
#include <iostream>
#include "include/Player.h"
#include "include/Map.h"
#include "include/constante.h"
#include <cmath>

void DisplayPerso(Player &player, SDL_Renderer* renderer);
void DisplayBackground(SDL_Renderer* renderer, SDL_Texture* skyTexture, SDL_Texture* groundTexture, int angle, int viewX, int viewY);
bool isCollision(int x, int y);
void cursor(SDL_Renderer* renderer);
SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer);

int main(int argc, char* args[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }


    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
            "DoomLike",              // titre
            SDL_WINDOWPOS_UNDEFINED,  // position initiale x
            SDL_WINDOWPOS_UNDEFINED,  // position initiale y
            width,                      // largeur, en pixels
            height,                      // hauteur, en pixels
            SDL_WINDOW_FULLSCREEN         // flags
    );

    // Rendre la souris invisible
    SDL_ShowCursor(SDL_DISABLE);

    // Activer le mode de mouvement relatif pour la souris
    SDL_SetRelativeMouseMode(SDL_TRUE);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Obtention du contexte de rendu
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Texture* wallTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/Wall.bmp", renderer);
    if (!wallTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return -1;
    }

    SDL_Texture* groundTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/ground.bmp", renderer);
    if (!groundTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return -1;
    }

    SDL_Texture* skyTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/sky.bmp", renderer);
    if (!skyTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return -1;
    }

    Player player{250,250};

    player.wallTexture = wallTexture;

    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks(), lastTime = startTime;

    SDL_Event event;
    bool running = true;
    while (running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                        case SDLK_q:
                        case SDLK_LEFT:
                            if (!isCollision(player.posX - sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY - cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += sin(player.angle * M_PI / 180) * vitesse;
                                player.posY += cos(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:  // Strafe right
                            if (!isCollision(player.posX + sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY + cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= sin(player.angle * M_PI / 180) * vitesse;
                                player.posY -= cos(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_z:
                        case SDLK_UP:  // Move forward
                            if (!isCollision(player.posX + cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY - sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += cos(player.angle * M_PI / 180) * vitesse;
                                player.posY -= sin(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:  // Move backward
                            if (!isCollision(player.posX - cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY + sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= cos(player.angle * M_PI / 180) * vitesse;
                                player.posY += sin(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    player.angle += event.motion.xrel * sensitivity;
                    player.angle = fmod(player.angle, 360.0);  // Normalize angle to 0-360 degrees
                    if (player.angle < 0) player.angle += 360.0;  // Adjust for negative turns
                    break;
            }
        }

        SDL_RenderClear(renderer);  // Clear the screen before new drawing

        // Drawing functions
        DisplayBackground(renderer, skyTexture, groundTexture, player.angle, player.posX, player.posY);
        player.line_view(renderer);
        DisplayPerso(player, renderer);
        cursor(renderer);

        SDL_RenderPresent(renderer);

        frameCount++;

        // Calculate and display FPS every second
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedMS = currentTime - lastTime;

        if (elapsedMS >= 1000) {
            float fps = frameCount / (elapsedMS / 1000.0f);
            // std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    // Nettoyage
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void DisplayPerso(Player &player, SDL_Renderer* renderer){
    SDL_Rect rect;
    rect.x = player.posX/MiniMap;
    rect.y = player.posY/MiniMap;
    rect.w = width/size_map/rapportPlayerMaps;
    rect.h = height/size_map/rapportPlayerMaps;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Texture* RotateTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture, double angle, int originalWidth, int originalHeight, int x , int y) {
    // Crée une texture de rendu pour le résultat
    SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, originalWidth, originalHeight);
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);

    // Définit cette texture comme cible de rendu
    SDL_SetRenderTarget(renderer, result);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Calcule le centre de la texture originale
    SDL_Point center = {x, y};

    // Dessine la texture originale sur la nouvelle texture cible avec rotation
    SDL_Rect srcRect = {0, 0, originalWidth, originalHeight};
    SDL_Rect destRect = {0, 0, originalWidth, originalHeight};
    SDL_RenderCopyEx(renderer, srcTexture, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);

    // Restaure la cible de rendu
    SDL_SetRenderTarget(renderer, NULL);

    return result;
}

void DisplayBackground(SDL_Renderer* renderer, SDL_Texture* skyTexture, SDL_Texture* groundTexture, int angle, int viewX, int viewY) {
    int skyTextureWidth, skyTextureHeight;
    SDL_QueryTexture(skyTexture, NULL, NULL, &skyTextureWidth, &skyTextureHeight);

    // Calcul de textureOffset pour le déplacement basé sur l'angle
    int textureOffset = (angle * 16) % skyTextureWidth;

    // Gestion du cas où le segment dépasse la fin de la texture
    int effectiveWidth = (textureOffset + 960 > skyTextureWidth) ? (skyTextureWidth - textureOffset) : 960;

    // Rectangle source pour la partie principale de la texture
    SDL_Rect srcRect1 = {
            textureOffset,
            0,
            effectiveWidth,
            skyTextureHeight
    };

    // Rectangle source pour la partie qui boucle si nécessaire
    SDL_Rect srcRect2 = {
            0,  // Début de la texture
            0,
            960 - effectiveWidth, // La partie restante pour compléter 960 pixels si nécessaire
            skyTextureHeight
    };

    // Rectangle destination pour srcRect1
    SDL_Rect destRect1 = {
            0,
            0,
            (effectiveWidth * width) / 960,  // Ajuster proportionnellement à la largeur de l'écran
            height / 2
    };

    // Rectangle destination pour srcRect2
    SDL_Rect destRect2 = {
            destRect1.w,  // À la fin de destRect1
            0,
            width - destRect1.w,  // Reste de la largeur de l'écran
            height / 2
    };

    // Afficher la texture principale
    SDL_RenderFillRect(renderer, &destRect1);
    SDL_RenderCopy(renderer, skyTexture, &srcRect1, &destRect1);

    // Afficher la partie bouclée si nécessaire
    if (srcRect2.w > 0) {
        SDL_RenderFillRect(renderer, &destRect2);
        SDL_RenderCopy(renderer, skyTexture, &srcRect2, &destRect2);
    }

    int textureWidth, textureHeight;
    SDL_QueryTexture(groundTexture, NULL, NULL, &textureWidth, &textureHeight);

    // Rotation de la texture
    SDL_Texture* rotatedTexture = RotateTexture(renderer, groundTexture, angle, textureWidth, textureHeight, viewX, viewY);

    // Calcul du rectangle source pour le sol avec zoom
    int visibleWidth = width / zoomGroundTexture;  // Largeur visible de la texture après application du zoom
    int visibleHeight = (height / 2) / zoomGroundTexture;  // Hauteur visible de la texture après application du zoom

    SDL_Rect srcRectGround = {
            viewX - width/2,
            viewY - height/2,
            visibleWidth,  // Réduire la largeur de la zone captée pour augmenter le zoom
            visibleHeight  // Réduire la hauteur de la zone captée pour augmenter le zoom
    };
    SDL_Rect destRect = {0, height / 2, width, height / 2};
    SDL_RenderCopy(renderer, rotatedTexture, &srcRectGround, &destRect);

    // Libération de la texture temporaire
    SDL_DestroyTexture(rotatedTexture);
}





bool isCollision(int x, int y) {
    std::array<int, 4> list_indice{
            (x+playerWidth)/(width/nb_case_w) + (y+playerHeight)/(height/nb_case_h)*nb_case_w,
            x/(width/nb_case_w) + y /(height/nb_case_h)*nb_case_w,
            (x+playerWidth)/(width/nb_case_w) + y /(height/nb_case_h)*nb_case_w,
            x/(width/nb_case_w) + (y+playerHeight)/(height/nb_case_h)*nb_case_w
    };

    if( map[list_indice[0]]==1 || map[list_indice[1]]==1||  map[list_indice[2]]==1||  map[list_indice[3]]==1  ){
        return true;
    }
    return false;
}

void cursor(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, width/2 - sizeCursor, height / 2, width / 2 + sizeCursor, height / 2);
    SDL_RenderDrawLine(renderer, width/2, height/2 - sizeCursor, width / 2, height / 2 + sizeCursor);
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
