#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


SDL_Point* copy_points(SDL_Point* points,int n);

void print_points_arr(SDL_Point* points, int n);

void swap(SDL_Point* points,int i, int j);

/*
Reads a file formatted as 

n (number of points)
x1;y1
x2;y2
.
.
xn;yn

and returns a SDL_Point* array which contains the points
*/
SDL_Point* build_arr(FILE* f,int* n);


void write_out(SDL_Point* points,int n);