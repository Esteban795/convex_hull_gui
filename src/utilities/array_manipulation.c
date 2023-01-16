#include "array_manipulation.h"

//Self explanatory..
SDL_Point* copy_points(SDL_Point* points,int n){
    SDL_Point* copy_arr = malloc(sizeof(SDL_Point) * n);
    for (int  i = 0;i < n;i++){
        SDL_Point p = {.x = points[i].x,.y = points[i].y};
        copy_arr[i] = p;
    }
    return copy_arr;
}

void print_points_arr(SDL_Point* points, int n){
    for (int i = 0; i < n;i++){
        printf("Point (%d,%d)",points[i].x,points[i].y);
    }
}

void swap(SDL_Point* points,int i, int j){
    SDL_Point temp = points[i];
    points[i] = points[j];
    points[j] = temp;
}



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
SDL_Point* build_arr(FILE* f,int* n){
    fscanf(f,"%d\n",n);
    SDL_Point* points = malloc(*n * sizeof(SDL_Point));
    int temp_x,temp_y;
    for (int i = 0; i < *n;i++){
        fscanf(f,"%d;%d\n",&temp_x,&temp_y);
        SDL_Point p = {.x = temp_x,.y = temp_y};
        points[i] = p;
    }
    return points;
}


void write_out(SDL_Point* points,int n){
    FILE* f = fopen("convex_hull_points.txt","r");
    fprintf(f,"%d\n",n);
    for (int i = 0; i < n;i++){
        fprintf(f,"%d %d",points[i].x,points[i].y);
    }
    fclose(f);
}
