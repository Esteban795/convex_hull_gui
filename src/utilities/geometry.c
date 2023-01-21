#include "geometry.h"


/*
Gives xmin,ymin, xmax,ymax coordinates of a bounding rectangle (generally not the minimal one).
*/
void bounding_box(SDL_Point* points,int n,int* width,int* height){
    int xmin = points[0].x;
    int xmax = points[0].x;
    int ymin = points[0].y;
    int ymax = points[0].y;
    for (int i = 1; i < n;i++){
        if (points[i].x < xmin) xmin = points[i].x;
        if (points[i].x > xmax) xmax = points[i].x;
        if (points[i].y < ymin) ymin = points[i].y;
        if (points[i].y > ymax) ymax = points[i].y;
    }
    *width = xmax - xmin + 20;
    *height = ymax - ymin + 20;
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
Modify points array to keep only points that form a unique angle with pivot.
*/
int ignore_colinear_points(SDL_Renderer* renderer,SDL_Point* points,int n){
    int new_len = 1;
    SDL_SetRenderDrawColor(renderer,0,0,255,128);
    for (int i = 1; i < n ;i++){
        while (i < n - 1 && orientation(pivot,points[i],points[i+ 1]) == 0){
            SDL_RenderDrawPoint(renderer,points[i].x,points[i].y);
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
int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer,camera* cam,SDL_Texture* texture,SDL_Rect dest){
    int index = 0;
    SDL_Point minimum = points[0];
    for (int i = 1; i < n;i++){
        poll_events(renderer,texture,cam,dest);
        if (points[i].y < minimum.y || (points[i].y == minimum.y && points[i].x < minimum.y)){
            index = i;
            minimum = points[i];
        }
    }
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    DrawCircle(renderer,minimum.x,minimum.y,RADIUS);
    update_screen(renderer,texture,*cam,dest);
    return index;
}