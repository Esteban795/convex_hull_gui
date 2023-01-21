#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "array_manipulation.h"
#include "../SDL_utilities/drawers.h"


#define WIDTH 800
#define HEIGHT 800
#define RADIUS 10
#define FPS 60

struct Camera {
    SDL_Rect source;
    int current_scale;
};

typedef struct Camera camera;

const SDL_Color orange = {255, 127, 40, 255};
const SDL_Color blue = {20, 20, 200, 255};
const SDL_Color red = {255, 10, 10, 255};
const SDL_Color green = {10, 255, 10, 255};
const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};

SDL_Point pivot;
/*
Sets up width and height parameters to the maximum distance in x and y axis.
*/
void bounding_box(SDL_Point* points,int n,int* width,int* height);

int dist_squared(SDL_Point A,SDL_Point B);

/* Find the orientation between 3 points.
# 0 --> A, B and C are collinear
# 1 --> Clockwise
# 2 --> Counterclockwise
*/
int orientation(SDL_Point A,SDL_Point B, SDL_Point C);

/*
Modify points array to keep only points that form a unique angle with pivot
*/
int ignore_colinear_points(SDL_Renderer* renderer,SDL_Point* points,int n);

bool comp(SDL_Point bottom_most,SDL_Point B); //lexical order with (y,x) coordinates, to find the bottom-most left point

/*
Searches for SDL_Point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer,camera* cam,SDL_Texture* texture,SDL_Rect dest);