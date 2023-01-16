#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../SDL_utilities/color_related.c"
#include "convex_hull_gui.h"

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
int ignore_colinear_points(SDL_Point* points,int n,SDL_Point pivot);

bool comp(SDL_Point bottom_most,SDL_Point B); //lexical order with (y,x) coordinates, to find the bottom-most left point

/*
Searches for SDL_Point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer,int radius);