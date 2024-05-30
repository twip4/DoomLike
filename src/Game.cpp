//
// Created by Paul Baudinot on 17/05/2024.
//

#include "../include/Game.h"

void DisplayPerso(Player &player, SDL_Renderer* renderer);
void DisplayMonster(const std::vector<Monster*>& listMonster, SDL_Renderer* renderer);
void DisplayBackground(SDL_Renderer* renderer, SDL_Texture* skyTexture, int angle);
bool isCollision(int x, int y);
void cursor(SDL_Renderer* renderer);
SDL_Texture* loadBMPTexure(const char* filepath, SDL_Renderer* renderer);
void displayHUD(SDL_Renderer* renderer, SDL_Texture* HUD);
int getRandomNumber(int min, int max);
void DisplayScore(SDL_Renderer* renderer, TTF_Font* font);
void DisplayText(SDL_Renderer* renderer, TTF_Font* font, const char *text, SDL_Color textColor);
void AnnimShot(SDL_Renderer* renderer, Textures T, auto now, bool* annimationBoucle) ;
void DisplayPv(SDL_Renderer* renderer, TTF_Font* font, Player player);
void updateMonsters(Monster* monster, Player* player);

int ModeSolo(SDL_Renderer* renderer, TTF_Font* font, Textures T){

    SDL_ShowCursor(SDL_DISABLE);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    std::vector<Monster*> listMonster;

    Player player{200,150,&listMonster};

    int round = 0;

    while(listMonster.size() <= nbMonsters[round]){
        int randomX = getRandomNumber(0, width-50);
        int randomY = getRandomNumber(0, height-50);
        if (!isCollision(randomX, randomY)) {
            Monster* monster = new Monster{randomX, randomY, T.monsterTexture}; // Allocation dynamique
            listMonster.push_back(monster);
            std::thread MonsterMove{&updateMonsters, monster, &player};
            MonsterMove.detach();
        }
    }

    bool click = false;
    bool annimationBoucle = false;
    auto now = std::chrono::steady_clock::now();;

    player.wallTexture = T.wallTexture;

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
                            return 0;
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
        player.view(renderer);
        player.lineCenter(renderer);
        DisplayPerso(player, renderer);

        DisplayMonster(listMonster, renderer);
        DisplayScore(renderer,font);
        DisplayPv(renderer,font,player);

        if(click){
            player.shot();
            now = std::chrono::steady_clock::now();
            annimationBoucle = true;
            click = false;
        }

        if(annimationBoucle){
            AnnimShot(renderer,T,now,&annimationBoucle);
        }
        else{
            displayHUD(renderer, T.HUDTexture);
        }

        cursor(renderer);
        SDL_RenderPresent(renderer);

        if (listMonster.empty()){
            if(round != 3){
                round += 1;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                DisplayText(renderer,font,"NEXT ROUND ", {255, 215, 0});
                SDL_RenderPresent(renderer);
                SDL_Delay(1000);

                for (Monster* monster : listMonster) {
                    delete monster;
                }

                while(listMonster.size() <= nbMonsters[round]){
                    int randomX = getRandomNumber(0, width-50);
                    int randomY = getRandomNumber(0, height-50);
                    if (!isCollision(randomX, randomY)) {
                        Monster* monster = new Monster{randomX, randomY, T.monsterTexture}; // Allocation dynamique
                        listMonster.push_back(monster);
                        std::thread MonsterMove{&updateMonsters, monster, &player};
                        MonsterMove.detach();
                    }
                }
            }
            else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                DisplayText(renderer,font,"YOU WIN !!!", {255, 215, 0});
                SDL_RenderPresent(renderer);
                SDL_Delay(1000);
                break;
            }
        }

        if(player.pv <= 0){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DisplayText(renderer,font,"YOU ARE DEAD", {227, 20, 20});
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            break;
        }
    }

    return 1;
}

void AnnimShot(SDL_Renderer* renderer, Textures T, auto now, bool* annimationBoucle) {
    auto end_time = std::chrono::steady_clock::now();
    auto duration = end_time - now;

    if (duration < std::chrono::milliseconds(delay)) {
        displayHUD(renderer, T.HUDSHOT1Texture);
    } else if (duration < std::chrono::milliseconds(delay * 2)) {
        displayHUD(renderer, T.HUDSHOT2Texture);
    } else if (duration < std::chrono::milliseconds(delay * 3)) {
        displayHUD(renderer, T.HUDSHOT3Texture);
    } else if (duration < std::chrono::milliseconds(delay * 4)) {
        displayHUD(renderer, T.HUDSHOT2Texture);
    } else {
        *annimationBoucle = false;
    }
}

void DisplayPerso(Player &player, SDL_Renderer* renderer){
    SDL_Rect rect;
    rect.x = (((float)player.posX/width)*ScreenWidth)/MiniMap;
    rect.y = (((float)player.posY/height)*ScreenWheight)/MiniMap;
    rect.w = width/size_map/rapportPlayerMaps;
    rect.h = height/size_map/rapportPlayerMaps;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void DisplayMonster(const std::vector<Monster*>& listMonster, SDL_Renderer* renderer){
    for (const auto &monster: listMonster) {
        SDL_Rect rect;
        rect.x = (((float)monster->posX/width)*ScreenWidth)/MiniMap;
        rect.y = (((float)monster->posY/height)*ScreenWheight)/MiniMap;
        rect.w = ScreenWidth/size_map/rapportPlayerMaps;
        rect.h = ScreenWheight/size_map/rapportPlayerMaps;

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
            (effectiveWidth * ScreenWidth) / 960,  // Ajuster proportionnellement à la largeur de l'écran
            ScreenWheight / 2
    };

    // Rectangle destination pour srcRect2
    SDL_Rect destRect2 = {
            destRect1.w,  // À la fin de destRect1
            0,
            ScreenWidth - destRect1.w,  // Reste de la largeur de l'écran
            ScreenWheight / 2
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

    SDL_Rect fillRect = {0, ScreenWheight/2, ScreenWidth, ScreenWheight/2};
    SDL_RenderFillRect(renderer, &fillRect);
}

bool isCollision(int x, int y) {
    std::array<int, 4> list_indice{
            (x+playerWidth)/(width/nb_case_w) + (y+playerHeight)/(height/nb_case_h)*nb_case_w,
            x/(width/nb_case_w) + y /(height/nb_case_h)*nb_case_w,
            (x+playerWidth)/(width/nb_case_w) + y /(height/nb_case_h)*nb_case_w,
            x/(width/nb_case_w) + (y+height)/(height/nb_case_h)*nb_case_w
    };

    if( map[list_indice[0]]==1 || map[list_indice[1]]==1||  map[list_indice[2]]==1||  map[list_indice[3]]==1  ){
        return true;
    }
    return false;
}

void cursor(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, ScreenWidth/2 - sizeCursor, ScreenWheight / 2, ScreenWidth / 2 + sizeCursor, ScreenWheight / 2);
    SDL_RenderDrawLine(renderer, ScreenWidth/2, ScreenWheight/2 - sizeCursor, ScreenWidth / 2, ScreenWheight / 2 + sizeCursor);
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
            ScreenWidth/2 ,
            ScreenWheight - HUDTextureHeight,
            ScreenWidth/2,
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
    std::string scoreText = "Score : ";
    scoreText += std::to_string(score);

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
    SDL_Rect renderQuad = {ScreenWidth - textureWidth, 0, textureWidth, textureHeight}; // Assuming 640 as window width

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void DisplayPv(SDL_Renderer* renderer, TTF_Font* font, Player player) {
    std::string scoreText = "PV : ";
    scoreText += std::to_string(player.pv);

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
    SDL_Rect renderQuad = {ScreenWidth - textureWidth - 200, 0, textureWidth, textureHeight}; // Assuming 640 as window width

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void DisplayText(SDL_Renderer* renderer, TTF_Font* font, const char *text, SDL_Color textColor){
    // Render the text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
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
    SDL_Rect renderQuad = {(ScreenWidth - textureWidth)/2, (ScreenWheight - textureHeight)/2, textureWidth, textureHeight}; // Assuming 640 as window width

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void updateMonsters(Monster* monster, Player* player) {
    while (monster) {

        if(monster->pv <= 0){
            monster = nullptr;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int stepX;
        int stepY;

        if (player->posX > monster->posX) {
            stepX = 5;
        } else {
            stepX = -5;
        }
        if (player->posY > monster->posY) {
            stepY = 5;
        } else {
            stepY = -5;
        }

        // Calculate new potential positions
        int newX = monster->posX + stepX;
        int newY = monster->posY + stepY;

        // Move the monster if no collision
        if (!isCollision(newX, newY)) {
            monster->move(stepX, stepY);
        }

        if((player->posX - monster->posX) > -5 && (player->posX - monster->posX) < 5 && (player->posY - monster->posY) > -5 && (player->posY - monster->posY) < 5){
            player->pv -= 5;
        }
    }
}