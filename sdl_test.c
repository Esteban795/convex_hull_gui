#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

const int TEXTURE_H = 2000;
const int TEXTURE_W = 2000;
const int SCREEN_H = 1000;
const int SCREEN_W = 1000;

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}
SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

int main(void){
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    SDL_Window* window= SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
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
    #include <SDL2/SDL.h>



    SDL_Rect source = {0,0,SCREEN_W/3,SCREEN_H/32};
    SDL_Rect dest = {10,10,SCREEN_W - 20,SCREEN_H - 20};
    SDL_Event e;
    SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,TEXTURE_W,TEXTURE_H);
    SDL_Point* points = malloc(sizeof(SDL_Point) * 1000);
    for (int i = 0; i < 1000;i++){
        int x = rand_between(0,TEXTURE_W);
        int y = rand_between(0,TEXTURE_H);
        SDL_Point point = {.x = x,.y = y};
        points[i] = point;
        SDL_RenderDrawPoint(renderer,x,y);
    }
    SDL_RenderPresent(renderer);
    int running = 1;
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    source.y -= 3;
                    break;
                case SDLK_DOWN:
                    source.y += 3;
                    break;
                case SDLK_LEFT:
                    source.x -= 3;
                    break;
                case SDLK_RIGHT:
                    source.x += 3;
                    break;
                case SDLK_1:
                    source.w *= 2;
                    source.h *= 2;
                    break;
                case SDLK_2:
                    source.w /= 2;
                    source.h /= 2;
                    break;
                default:
                    break;
                }
            }
        }
        SDL_SetRenderTarget(renderer,texture);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        /*
        for (int i = 0; i < 1000;i++){
            points[i].x += rand_between(0,4) % 3 - 1;
            points[i].y += rand_between(0,4) % 3 - 1;
            SDL_RenderDrawPoint(renderer,points[i].x,points[i].y
        }
        */
        SDL_SetRenderDrawColor(renderer,0,255,255,255);
        SDL_RenderDrawPoints(renderer,points,1000);

        SDL_SetRenderTarget(renderer,NULL);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderCopy(renderer,texture,&source,&dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }
    SDL_Delay(5000);
    free(points);
    /* On agit sur la fenêtre ici */
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if  (window != NULL) SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}

//gcc sdl_test.c -o sdl_test -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2_image
