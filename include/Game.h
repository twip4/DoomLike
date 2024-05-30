//
// Created by Paul Baudinot on 17/05/2024.
//

#ifndef DOOMLIKE_GAME_H
#define DOOMLIKE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <random>
#include "../include/Map.h"
#include "../include/constante.h"
#include "../include/Game.h"
#include "../include/Textures.h"
#include "../include/Player.h"
#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>

struct Node {
    int x, y;
    float gCost, hCost;
    Node* parent;

    Node(int x, int y, Node* parent = nullptr)
            : x(x), y(y), gCost(0), hCost(0), parent(parent) {}

    float fCost() const {
        return gCost + hCost;
    }

    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

int ModeSolo(SDL_Renderer* renderer, TTF_Font* font, Textures T);
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
void updateMonsters2(Monster* monster,Player* player);
void AnnimShot(SDL_Renderer* renderer, Textures T, auto now, bool* annimationBoucle) ;
void DisplayPv(SDL_Renderer* renderer, TTF_Font* font, Player player);
void updateMonsters(Monster* monster, Player* player);

#endif //DOOMLIKE_GAME_H
