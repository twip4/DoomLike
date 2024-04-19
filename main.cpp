#include <SDL2/SDL.h>
#include <iostream>
#include "include/Player.h"
#include "include/Map.h"
#include "include/constante.h"
#include <cmath>


void DisplayMap(SDL_Renderer* renderer);
void DisplayPerso(Player &player, SDL_Renderer* renderer);
void DisplayBackground(SDL_Renderer* renderer);
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
            SDL_WINDOW_SHOWN         // flags
    );

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

    SDL_Texture* myTexture = loadBMPTexure("/Users/paulbaudinot/CLionProjects/DoomLike/assets/Wall.bmp", renderer);
    if (!myTexture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return -1;
    }

    Player player{70,70};

    player.myTexture = myTexture;

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
        DisplayBackground(renderer);
        player.line_view(renderer);
        DisplayMap(renderer);
        DisplayPerso(player, renderer);
        player.lineCenter(renderer);
        cursor(renderer);

        SDL_RenderPresent(renderer);

        frameCount++;

        // Calculate and display FPS every second
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedMS = currentTime - lastTime;

        if (elapsedMS >= 1000) {
            float fps = frameCount / (elapsedMS / 1000.0f);
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    // Nettoyage
    SDL_DestroyTexture(myTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void DisplayMap(SDL_Renderer* renderer){
    for(int i=0;i<nb_case_w*nb_case_h;i++){
            SDL_Rect rect;
            rect.x = i%nb_case_w*width/nb_case_w/MiniMap;
            rect.y = floor(i/nb_case_h)*height/nb_case_h/MiniMap;
            rect.w = width/size_map/MiniMap;
            rect.h = height/size_map/MiniMap;
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

void DisplayPerso(Player &player, SDL_Renderer* renderer){
    SDL_Rect rect;
    rect.x = player.posX/MiniMap;
    rect.y = player.posY/MiniMap;
    rect.w = width/size_map/rapportPlayerMaps;
    rect.h = height/size_map/rapportPlayerMaps;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void DisplayBackground(SDL_Renderer* renderer){
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 1000;
    rect.h = 500;

    SDL_SetRenderDrawColor(renderer, 91, 228, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    rect.x = 0;
    rect.y = 500;
    rect.w = 1000;
    rect.h = 500;

    SDL_SetRenderDrawColor(renderer, 105, 55, 6, 255);
    SDL_RenderFillRect(renderer, &rect);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, width/2-sizeCursoe, width/2, width/2+sizeCursoe, width/2);
    SDL_RenderDrawLine(renderer, width/2, width/2-sizeCursoe, width/2, width/2+sizeCursoe);
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
