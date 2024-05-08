#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <random>
#include "include/Player.h"
#include "include/Monster.h"
#include "include/Map.h"
#include "include/constante.h"
#include "include/Textures.h"
#include <cmath>
#include "vector"

void DisplayPerso(Player &player, SDL_Renderer* renderer);
void DisplayMonster(const std::vector<Monster>& listMonster, SDL_Renderer* renderer);
void DisplayBackground(SDL_Renderer* renderer, SDL_Texture* skyTexture, int angle);
bool isCollision(int x, int y);
void cursor(SDL_Renderer* renderer);
SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer);
void displayHUD(SDL_Renderer* renderer, SDL_Texture* HUD);
int getRandomNumber(int min, int max);
void DisplayScore(SDL_Renderer* renderer, TTF_Font* font);

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

    width = dm.w; // Mettre à jour la valeur de width
    height = dm.h; // Mettre à jour la valeur de height

    SDL_Window* window = SDL_CreateWindow(
            "DoomLike",                        // titre
            SDL_WINDOWPOS_UNDEFINED,           // position initiale x
            SDL_WINDOWPOS_UNDEFINED,           // position initiale y
            width,                             // largeur, utilisez la largeur de l'écran
            height,                             // hauteur, utilisez la hauteur de l'écran
            SDL_WINDOW_FULLSCREEN_DESKTOP      // flags
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

    Textures T = Textures(renderer);

    std::vector<Monster> listMonster;

    while(listMonster.size() <= 10){
        int randomX = getRandomNumber(0, width);
        int randomY = getRandomNumber(0, height);
        if (!isCollision(randomX,randomY)){
            Monster monster{randomX,randomY,T.monsterTexture};
            listMonster.push_back(monster);
        }
    }

    Player player{200,150,&listMonster};

    bool click = false;

    player.wallTexture = T.wallTexture;

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
                            if (!isCollision(player.posX + sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY - cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += sin(player.angle * M_PI / 180) * vitesse;
                                player.posY -= cos(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:  // Strafe right
                            if (!isCollision(player.posX - sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY + cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= sin(player.angle * M_PI / 180) * vitesse;
                                player.posY += cos(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_z:
                        case SDLK_UP:  // Move forward
                            if (!isCollision(player.posX + cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY + sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += cos(player.angle * M_PI / 180) * vitesse;
                                player.posY += sin(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:  // Move backward
                            if (!isCollision(player.posX - cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY - sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= cos(player.angle * M_PI / 180) * vitesse;
                                player.posY -= sin(player.angle * M_PI / 180) * vitesse;
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    player.angle += event.motion.xrel * sensitivity;
                    player.angle = fmod(player.angle, 360.0);  // Normalize angle to 0-360 degrees
                    if (player.angle < 0) player.angle += 360.0;  // Adjust for negative turns
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    click = true;
            }
        }

        SDL_RenderClear(renderer);  // Clear the screen before new drawing

        DisplayBackground(renderer, T.skyTexture, player.angle);
        player.line_view(renderer);
        player.lineCenter(renderer);
        DisplayPerso(player, renderer);
        DisplayMonster(listMonster, renderer);
        DisplayScore(renderer,font);
        if(click){
            displayHUD(renderer, T.HUD_FIRETexture);
            player.shot();
            SDL_Delay(200);
            click = false;
        }
        else{
            displayHUD(renderer, T.HUDTexture);
        }

        cursor(renderer);

        std::cout << score << std::endl;

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
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(T.wallTexture);
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void DisplayMonster(const std::vector<Monster>& listMonster, SDL_Renderer* renderer){
    for (const auto &monster: listMonster) {
        SDL_Rect rect;
        rect.x = monster.posX/MiniMap;
        rect.y = monster.posY/MiniMap;
        rect.w = width/size_map/rapportPlayerMaps;
        rect.h = height/size_map/rapportPlayerMaps;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void DisplayBackground(SDL_Renderer* renderer, SDL_Texture* skyTexture, int angle) {
    int skyTextureWidth, skyTextureHeight;
    SDL_QueryTexture(skyTexture, NULL, NULL, &skyTextureWidth, &skyTextureHeight);

    // Calcul de textureOffset pour le déplacement basé sur l'angle
    int textureOffset = skyTextureWidth + ((angle * 16) % skyTextureWidth);

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

    SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);

    SDL_Rect fillRect = {0, height/2, width, height/2};
    SDL_RenderFillRect(renderer, &fillRect);
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

void displayHUD(SDL_Renderer* renderer, SDL_Texture* HUD){
    int HUDTextureWidth, HUDTextureHeight;
    SDL_QueryTexture(HUD, NULL, NULL, &HUDTextureWidth, &HUDTextureHeight);

    SDL_Rect srcRect = {
            0,
            0,
            HUDTextureWidth,
            HUDTextureHeight
    };

    SDL_Rect destRect = {
            0,
            height - HUDTextureHeight,
            width,
            HUDTextureHeight
    };

    SDL_RenderCopy(renderer, HUD, &srcRect, &destRect);
}

int getRandomNumber(int min, int max) {
    static std::random_device rd;  // Obtain a random number from hardware
    static std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(min, max); // Define the range

    return distr(gen); // Generate numbers
}

void DisplayScore(SDL_Renderer* renderer, TTF_Font* font) {
    // Convert score to string
    std::string scoreText = std::to_string(score);

    // Color for the text
    SDL_Color textColor = {255, 255, 255}; // White

    // Render the text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface == nullptr) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface); // Free the surface immediately after use
        return;
    }

    // Get the texture width and height
    int textureWidth, textureHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textureWidth, &textureHeight);

    // Define the position and dimensions for the texture on the renderer
    SDL_Rect renderQuad = {width - textureWidth, 0, textureWidth, textureHeight}; // Assuming 640 as window width

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}