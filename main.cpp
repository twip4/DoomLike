#include <SDL.h>
#include <iostream>

int main(int argc, char* args[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
            "SDL2 Test",              // titre
            SDL_WINDOWPOS_UNDEFINED,  // position initiale x
            SDL_WINDOWPOS_UNDEFINED,  // position initiale y
            800,                      // largeur, en pixels
            600,                      // hauteur, en pixels
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

    // Boucle d'événements
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Couleur de fond
        SDL_SetRenderDrawColor(renderer, 0x1E, 0x90, 0xFF, 0xFF); // Bleu dodger
        SDL_RenderClear(renderer);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
