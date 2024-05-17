//
// Created by Paul Baudinot on 17/05/2024.
//

#include "../include/Menu.h"

void displayText(SDL_Renderer* renderer, TTF_Font* font,int x,int y,const char *text,SDL_Color textColor);
void displayRectangle(SDL_Renderer* renderer, int x, int y, int rWidth, int rHeight, SDL_Color color);
bool isInsideRectangle(int x, int y, int rectX, int rectY, int rectW, int rectH);

int Accueil(SDL_Renderer* renderer, TTF_Font* font) {
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
                    }
                case SDL_MOUSEBUTTONDOWN:
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    if (isInsideRectangle(mouseX, mouseY, width/2 - (width/2)/2, height/6 * 2, width/2, height/12)) {
                        return 1;
                    }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderClear(renderer);

            displayText(renderer,font,width/2,height/5,"Menu principal",{255,255,255});

            displayRectangle(renderer,width/2 - (width/2)/2,height/6 * 2,width/2,height/12,{255,255,255});

            displayText(renderer,font,width/2,(height/6 * 2) + height/12/2 ,"Mode Solo",{0,0,0});

            displayRectangle(renderer,width/2 - (width/2)/2,height/6 * 3,width/2,height/12,{255,255,255});

            displayText(renderer,font,width/2,(height/6 * 3) + height/12/2,"Mode Multi",{0,0,0});

            SDL_RenderPresent(renderer);

        }
    }
    return 0;
}

void displayText(SDL_Renderer* renderer, TTF_Font* font,int x,int y,const char *text,SDL_Color textColor){
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
    SDL_Rect renderQuad = {x - textureWidth/2, y - textureHeight/2, textureWidth, textureHeight};

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void displayRectangle(SDL_Renderer* renderer, int x, int y, int rWidth, int rHeight, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = rWidth;
    rect.h = rHeight;

    SDL_RenderFillRect(renderer, &rect);
}

bool isInsideRectangle(int x, int y, int rectX, int rectY, int rectW, int rectH) {
    return (x > rectX && x < (rectX + rectW) && y > rectY && y < (rectY + rectH));
}