#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#define WIDTH 400
#define HEIGHT 400

SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

/*
void draw_rects(SDL_Renderer* renderer,SDL_Rect* rects,int n){
    for (int i = 0; i < n;i++){
        SDL_Color c = {(i + 4) * 10,(i + 4) * 10,(i + 4) * 10,255};
        SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
        SDL_RenderDrawRect(renderer,&rects[i]);
        SDL_RenderFillRect(renderer,&rects[i]);
    }
    SDL_RenderPresent(renderer);
}

SDL_Rect* generate_rects(int n){
    SDL_Rect* rects = malloc(sizeof(SDL_Rect) * n);
    for (int i = 0;i < n;i++){
        SDL_Rect r = {(i + 1)* 20,(i + 1)* 20,(i + 3)* 20,(i + 3)* 20};
        rects[i] = r;
    }
    return rects;
}
*/

SDL_Rect camera = {WIDTH/2 - WIDTH/4,HEIGHT/2 - HEIGHT/4,200,200};

int main(int argc, char *argv[]){
    SDL_Color orange = {255, 127, 40, 255};
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    SDL_Window* window= SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(NULL == renderer){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    if(0 != SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    int width = 100;
    int height = 100;
    int color = 1;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,300,300);
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for (int i = 0; i < 8;i++){
        for (int j = 0; j < 8;j++){
            if (color == 1){
                SDL_SetRenderDrawColor(renderer,0,255,0,255);
                color = 0;
            }
            else {
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                color = 1;
            }
            int x = j * 100;
            int y = i * 100;
            SDL_Rect r = {x,y,width - 5,height - 5};
            SDL_RenderFillRect(renderer,&r);
            
        } 
        if (color == 1){
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            color = 0;
        }
        else {
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            color = 1;
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_Rect clip = {10,10,200,200};
    SDL_Rect position = {150,150,200,200};
    SDL_RenderCopy(renderer,texture,NULL,&position);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    /* On agit sur la fenêtre ici */
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if  (window != NULL) SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}

// gcc sdl_test.c -Wvla -Wall -Wextra -fsanitize=address $(sdl2-config --cflags --libs) -o sdl_test
