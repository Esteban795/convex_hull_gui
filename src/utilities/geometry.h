#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <SDL2/SDL.h>
#include <stdbool.h>


#define WIDTH 800
#define HEIGHT 800
#define RADIUS 10
#define IPS 60

SDL_Point pivot;
int en_cours = 1;


struct Camera {
    SDL_Rect source;
    int current_scale;
};

typedef struct Camera camera;



void update_screen(SDL_Renderer* renderer,SDL_Texture* texture,camera* cam,SDL_Rect dest);

void poll_events(SDL_Renderer* renderer,SDL_Texture* texture,camera* cam,SDL_Rect dest);


int dist_squared(SDL_Point A,SDL_Point B);

int orientation(SDL_Point A,SDL_Point B, SDL_Point C);

int ignore_colinear_points(SDL_Renderer* renderer,SDL_Point* points,int n);

void bounding_box(SDL_Point* points,int n,int* width,int* height);

bool comp(SDL_Point bottom_most,SDL_Point B);

int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer,camera* cam,SDL_Texture* texture,SDL_Rect dest);

#endif