#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "utilities/stack.c"
#include "SDL_utilities/color_related.c"
#include "utilities/array_manipulation.c"

#define WIDTH 1500
#define HEIGHT 1000
#define RADIUS 3 


SDL_Point pivot;


struct result {
    SDL_Point* arr;
    int len;
};

typedef struct result result;

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title);