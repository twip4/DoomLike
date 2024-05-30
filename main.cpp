#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "include/constante.h"
#include "include/Textures.h"
#include "include/Game.h"
#include "include/MultiPlayer.h"
#include "include/Menu.h"

int main(int argc, char* args[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("/Users/paulbaudinot/CLionProjects/DoomLike/assets/Minecraft.ttf", 24);
    if (font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm); // Obtenez le mode d'affichage du bureau (0 pour le premier écran)

    ScreenWidth = dm.w; // Mettre à jour la valeur de width
    ScreenWheight = dm.h; // Mettre à jour la valeur de height

    SDL_Window* window = SDL_CreateWindow(
            "DoomLike",                        // titre
            SDL_WINDOWPOS_UNDEFINED,           // position initiale x
            SDL_WINDOWPOS_UNDEFINED,           // position initiale y
            ScreenWidth,                             // largeur, utilisez la largeur de l'écran
            ScreenWheight,                             // hauteur, utilisez la hauteur de l'écran
            SDL_WINDOW_FULLSCREEN_DESKTOP      // flags
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

    Textures T = Textures(renderer);

    int run = 1;

    while(run){
        run = Accueil(renderer, font);
        if (!run){
            break;
        }
        if(run == 1){
            run = ModeSolo(renderer,font,T);
        }
        if(run == 2){
            run = multi(renderer,font,T);
        }
    }

    // Nettoyage
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(T.wallTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

