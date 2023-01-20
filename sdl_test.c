#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

const int TEXTURE_H = 6000;
const int TEXTURE_W = 6000;
const int SCREEN_H = 800;
const int SCREEN_W = 800;

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}

struct Camera {
    SDL_Rect source;
    int current_scale;
};

typedef struct Camera camera;

void print_cam(camera cam){
    printf("Camera : x : %d, y : %d,width : %d, height %d, current_scale : %d\n",cam.source.x,cam.source.y,cam.source.w,cam.source.h,cam.current_scale);
}

void update_screen(SDL_Renderer* renderer,SDL_Texture* texture,camera cam,SDL_Rect dest){
    SDL_SetRenderTarget(renderer,NULL);
    SDL_RenderCopy(renderer,texture,&(cam.source),&dest);
    SDL_RenderPresent(renderer);
    SDL_Delay(17);
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius){
   const int32_t diameter = (radius * 2);
   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);
   while (x >= y){
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
      if (error <= 0){
        ++y;
        error += ty;
        ty += 2;
      }
      if (error > 0){
        --x;
        tx += 2;
        error += (tx - diameter);
      }
   }
}


void poll_events(camera* cam){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN) {
                print_cam(*cam);
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (cam->source.y < -2) break;
                    cam->source.y -= 3;
                    break;
                case SDLK_DOWN:
                    if (cam->source.y > SCREEN_H + 2) break;
                    cam->source.y += 3;
                    break;
                case SDLK_LEFT:
                    if (cam->source.x < -2) break;
                    cam->source.x -= 5;
                    break;
                case SDLK_RIGHT:
                    if (cam->source.x > SCREEN_W + 2) break;
                    cam->source.x += 5;
                    break;
                case SDLK_1:
                    cam->source.w *= 2;
                    cam->source.h *= 2;
                    cam->current_scale /= 2;
                    break;
                case SDLK_2:
                    cam->source.w /= 2;
                    cam->source.h /= 2;
                    cam->current_scale *= 2;
                    break;
                default:
                    break;
                }
            }
        }
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



    camera cam = {.source = {0,0,SCREEN_W/8,SCREEN_H/8},.current_scale = 4};
    SDL_Rect dest = {10,10,SCREEN_W - 20,SCREEN_H - 20};
    SDL_Event e;
    int n = 1000;
    SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,TEXTURE_W,TEXTURE_H);
    SDL_Point* points = malloc(sizeof(SDL_Point) * n);
    for (int i = 0; i < n;i++){
        int x = rand_between(0,TEXTURE_W);
        int y = rand_between(0,TEXTURE_H);
        SDL_Point point = {.x = x,.y = y};
        points[i] = point;
    }
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for (int i = 0; i < n;i++){
        int x = rand_between(0,TEXTURE_W);
        int y = rand_between(0,TEXTURE_H);
        SDL_Point point = {.x = x,.y = y};
        points[i] = point;
        DrawCircle(renderer,x,y,20);
    }

    SDL_SetRenderTarget(renderer,NULL);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderCopy(renderer,texture,NULL,&dest);
    SDL_RenderPresent(renderer);
    for (int i = 0; i < n - 1;i++){
        SDL_SetRenderTarget(renderer,texture);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        poll_events(&cam);
        SDL_RenderDrawLine(renderer,points[i].x,points[i].y,points[i + 1].x,points[i + 1].y);
        
        update_screen(renderer,texture,cam,dest);
    }
    SDL_RenderPresent(renderer);
    /*
    while (running){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (source.y < -2) break;
                    source.y -= 3;
                    break;
                case SDLK_DOWN:
                    if (source.y > SCREEN_H + 2) break;
                    source.y += 3;
                    break;
                case SDLK_LEFT:
                    if (source.x < -2) break;
                    source.x -= 5;
                    break;
                case SDLK_RIGHT:
                    if (source.x > SCREEN_W + 2) break;
                    source.x += 5;
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
        
        for (int i = 0; i < 1000;i++){
            points[i].x += rand_between(0,4) % 3 - 1;
            points[i].y += rand_between(0,4) % 3 - 1;
        }
        
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderDrawPoints(renderer,points,1000);

        SDL_SetRenderTarget(renderer,NULL);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderCopy(renderer,texture,&source,&dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(17);
    }
    */
    SDL_Delay(5000);
    SDL_DestroyTexture(texture);
    free(points);
    /* On agit sur la fenêtre ici */
    Quit:
        if (renderer != NULL) SDL_DestroyRenderer(renderer);
        if  (window != NULL) SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}

//gcc sdl_test.c -o sdl_test -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2_image
