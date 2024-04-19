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

    Player player{70,70};

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
                            player.angle+= v_angle;
                            break;
                        case SDLK_RIGHT:
                            player.angle-= v_angle;
                            break;
                        case SDLK_UP:
                            if(!isCollision(player.posX+cos(player.angle*PI/180)*vitesse, player.posY-sin(player.angle*PI/180)*vitesse)){
                                player.posX+=cos(player.angle*PI/180)*vitesse;
                                player.posY-=sin(player.angle*PI/180)*vitesse;
                            }
                            break;
                        case SDLK_DOWN:
                            if(!isCollision(player.posX-cos(player.angle*PI/180)*vitesse,player.posY+sin(player.angle*PI/180)*vitesse) ){
                                player.posX-=cos(player.angle*PI/180)*vitesse;
                                player.posY+=sin(player.angle*PI/180)*vitesse;
                            }
                            break;
                    }
                    break;
            }
        }

        DisplayBackground(renderer);
        DisplayMap(renderer);
        DisplayPerso(player, renderer);
        for (float i = player.angle - fov/2; i < player.angle + fov/2; i += precision_angle){
            player.line_view(renderer,i);
        }
        player.lineCenter(renderer);


        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / fps);
    }

    // Nettoyage
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
