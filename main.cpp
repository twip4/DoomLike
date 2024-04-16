#include <SDL2/SDL.h>
#include <iostream>
#include "include/Player.h"
#include "include/Map.h"
#include "include/constante.h"


void DisplayMap(Map &map,SDL_Renderer* renderer);
void DisplayPerso(Player &player, SDL_Renderer* renderer);

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

    Player player{0,0,&map};

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
                            player.SetPosition(-vitesse, 0);
                            break;
                        case SDLK_RIGHT:
                            player.SetPosition(vitesse, 0);
                            break;
                        case SDLK_UP:
                            player.SetPosition(0, -vitesse);
                            break;
                        case SDLK_DOWN:
                            player.SetPosition(0, vitesse);
                            break;
                    }
                    break;
            }
        }
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
            rect.x = width/size_map*i;
            rect.y = height/size_map*j;
            rect.w = width/size_map;
            rect.h = height/size_map;
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
    rect.x = player.Posx;
    rect.y = player.Posy;
    rect.w = width/size_map/rapportPlayerMaps;
    rect.h = height/size_map/rapportPlayerMaps;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}