#include <SDL2/SDL.h>
#include <iostream>
#include "include/Player.h"
#include "include/Map.h"
#include "include/constante.h"
#include <cmath>




void DisplayMap(Map &map,SDL_Renderer* renderer);
void DisplayPerso(Player &player, SDL_Renderer* renderer);
void DisplayBackground(SDL_Renderer* renderer);

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
            SDL_WINDOW_SHOWN          // flags
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

    Map map(size_map,size_map);
    map.init("../assets/map.txt");

    Player player{150,150,&map};

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            player.angle -= 5;
                            if (player.angle < 0) player.angle += 360; // If angle goes negative, wrap it around
                            break;
                        case SDLK_RIGHT:
                            player.angle += 5;
                            player.angle %= 360; // Wrap around if the angle exceeds 359
                            break;
                        case SDLK_UP:
                            player.SetPosition(vitesse);
                            break;
                        case SDLK_DOWN:
                            player.SetPosition(-vitesse);
                            break;
                    }
                    break;
            }
        }

        DisplayBackground(renderer);
        player.TraceRayon(renderer);
        DisplayMap(map, renderer);
        DisplayPerso(player, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 120); // Délai pour ~60 FPS
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void DisplayMap(Map &map,SDL_Renderer* renderer){
    for(int i = 0; i < map.getWidth(); i++){
        for(int j = 0; j < map.getHeight(); j++){
            SDL_Rect rect;
            rect.x = width/size_map/MiniMap*i;
            rect.y = height/size_map/MiniMap*j;
            rect.w = width/size_map/MiniMap;
            rect.h = height/size_map/MiniMap;
            if(map.getTile(i,j)){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else{
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void DisplayPerso(Player &player, SDL_Renderer* renderer){
    SDL_Rect rect;
    rect.x = player.posX/MiniMap;
    rect.y = player.posY/MiniMap;
    rect.w = width/size_map/rapportPlayerMaps/MiniMap;
    rect.h = height/size_map/rapportPlayerMaps/MiniMap;

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


