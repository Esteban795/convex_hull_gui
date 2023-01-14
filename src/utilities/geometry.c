#include "geometry.h"


/*
Gives xmin,ymin, xmax,ymax coordinates of a bounding rectangle (generally not the minimal one).
*/
void bounding_box(SDL_Point* points,int n,int* xmin,int* xmax,int* ymin,int* ymax){
    *xmin = points[0].x;
    *xmax = points[0].x;
    *ymin = points[0].y;
    *ymax = points[0].y;
    for (int i = 1; i < n;i++){
        if (points[i].x < *xmin) *xmin = points[i].x;
        if (points[i].x > *xmax) *xmax = points[i].x;
        if (points[i].y < *ymin) *ymin = points[i].y;
        if (points[i].y > *ymax) *ymax = points[i].y;
    }
}


int dist_squared(SDL_Point A,SDL_Point B){
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y)* (A.y - B.y);
}


/* Find the orientation between 3 points.
# 0 --> A, B and C are collinear
# 1 --> Clockwise
# 2 --> Counterclockwise
*/
int orientation(SDL_Point A,SDL_Point B, SDL_Point C){
    int res = (B.y - A.y) * (C.x - B.x) - (B.x - A.x) * (C.y - B.y);
    if (res == 0) return 0;
    if (res > 0) return 1;
    if (res < 0) return 2;
}


/*
Modify points array to keep only points that form a unique angle with pivot
*/
int ignore_colinear_points(SDL_Point* points,int n,SDL_Point pivot){
    int new_len = 1;
    for (int i = 1; i < n ;i++){
        while (i < n - 1 && orientation(pivot,points[i],points[i+ 1]) == 0){
            i++;
        }
        points[new_len] = points[i];
        new_len++;
    }
    return new_len;
}



bool comp(SDL_Point bottom_most,SDL_Point B){ //lexical order with (y,x) coordinates, to find the bottom-most left point 
    if (bottom_most.y < B.y) {
        return false;
    } else if (bottom_most.y == B.y && B.x > bottom_most.x){
        return false;
    }
    return true;
}


/*
Searches for SDL_Point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer,int radius){
    int index = 0;
    SDL_Point minimum = points[0];
    for (int i = 1; i < n;i++){
        if (points[i].y < minimum.y || (points[i].y == minimum.y && points[i].x < minimum.y)){
            index = i;
            minimum = points[i];
        }
    }
    DrawCircle(renderer,minimum.x,minimum.y,green,radius);
    SDL_RenderPresent(renderer);
    return index;
}



/*
Sorts an array of points with respect to the first point.
*/
int compare_qsort(const void* A,const void* B){ 
    SDL_Point a = *(SDL_Point*)A;
    SDL_Point b = *(SDL_Point*)B;
    int o = orientation(pivot,a,b);
    if (o == 0){ //colinear
        if (dist_squared(pivot,b) >= dist_squared(pivot,a)) return -1;
        return 1;
    }
    if (o == 2) return -1;
    if (o == 1) return 1;
}