#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

int setWindowColor(SDL_Renderer *renderer, SDL_Color color);