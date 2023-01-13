#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "stack.c"

#define WIDTH 1500
#define HEIGHT 1000
#define RADIUS 3 

/*
Animation of how Graham Scan algorithm actually works behind the scenes.
Make sure the file you're passing as an argument has the following pattern : 

n (which is the number of points to consider)
x1 y1
x2 y2
x3 y3 
.
.
.
xn yn

Not respecting this format will result in undefined behaviour.
*/

SDL_Point pivot;


struct result {
    SDL_Point* arr;
    int len;
};

typedef struct result result;


SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};


void swap(SDL_Point* points,int i, int j){
    SDL_Point temp = points[i];
    points[i] = points[j];
    points[j] = temp;
}
//// SDL

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}

////

//// array_manipulation
SDL_Point* copy_points_arr(SDL_Point* points,int n){
    SDL_Point* copy_arr = malloc(sizeof(SDL_Point) * n);
    for (int  i = 0;i < n;i++){
        SDL_Point p = {.x = points[i].x,.y = points[i].y};
        copy_arr[i] = p;
    }
    return copy_arr;
}

/////
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY,SDL_Color color){
   const int32_t diameter = (RADIUS * 2);
   int32_t x = (RADIUS- 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);
   SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
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

/*
Gives xmin,ymin, xmax,ymax coordinates of a bounding rectangle (generally not the minimal one)
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
////



int dist(SDL_Point A,SDL_Point B){
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y)* (A.y - B.y);
}

bool comp(SDL_Point A,SDL_Point B){//lexical order with (y,x) coordinates
    if (A.y < B.y) {
        return false;
    } else if (A.y == B.y && B.x > A.x){
        return false;
    }
    return true;
}

/* Find the orientation between 3 points.
# 0 --> A, B and C are collinear
# 1 --> Clockwise
# 2 --> Counterclockwise
*/
int orientation(SDL_Point A,SDL_Point B, SDL_Point C){
    int res = ((B.y - A.y) * (C.x - B.x) - (B.x - A.x) * (C.y - B.y));
    if (res == 0) return 0;
    if (res > 0) return 1;
    if (res < 0) return 2;
}

/*
Sorts an array of points with respect to the first SDL_Point.
*/
int compare_qsort(const void* A,const void* B){ 
    SDL_Point a = *(SDL_Point*)A;
    SDL_Point b = *(SDL_Point*)B;
    int o = orientation(pivot,a,b);
    if (o == 0){ //colinear
        if (dist(pivot,b) >= dist(pivot,a)) return -1;
        return 1;
    }
    if (o == 2) return -1;
    if (o == 1) return 1;
}




/*
Searches for SDL_Point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(SDL_Point* points,int n,SDL_Renderer* renderer){ //lexical order with (y,x) ok
    int index = 0;
    SDL_Point minimum = points[0];
    for (int i = 1; i < n;i++){
        if (points[i].y < minimum.y || (points[i].y == minimum.y && points[i].x < minimum.y)){
            index = i;
            minimum = points[i];
        }
    }
    DrawCircle(renderer,minimum.x,minimum.y,green);
    SDL_RenderPresent(renderer);
    return index;
}

/*
Modify points array to keep only points that form a unique angle with pivot
*/
int remove_same_angle_points(SDL_Point* points,int n){
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


int apply_effect_on_condition(stack* s,SDL_Renderer* renderer,SDL_Point* points,int i){
    SDL_Point top = points[stack_peek(s)];
    SDL_Point next_to_top = points[stack_peek_second(s)];
    SDL_SetRenderDrawColor(renderer,blue.r,blue.g,blue.b,blue.a);
    SDL_RenderDrawLine(renderer,top.x,top.y,points[i].x,points[i].y);
    SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
    SDL_RenderPresent(renderer);
    int o = orientation(next_to_top,top,points[i]);
    if (o != 2){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_Delay(25);
        SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
        SDL_RenderDrawLine(renderer,points[i].x,points[i].y,top.x,top.y);
        SDL_RenderPresent(renderer);
    }
    printf("\n\n\n");
    return o;
}


result graham_scan(SDL_Point* points,int n,SDL_Renderer* renderer){
    int piv = find_pivot(points,n,renderer);
    SDL_Delay(1000);
    pivot = points[piv];
    swap(points,0,piv);
    qsort(points,n,sizeof(SDL_Point),compare_qsort);
    //We now need to remove points that have the same angle, and only keep the farthest from pivot ones.
    int new_len = remove_same_angle_points(points,n);
    stack* s = stack_new();
    stack_push(s,0);
    stack_push(s,1);
    stack_push(s,2);
    for (int i = 3; i < new_len;i++){
        while (s->len > 1 && apply_effect_on_condition(s,renderer,points,i) != 2) {
            stack_pop(s);
        }
        SDL_RenderDrawLine(renderer,points[stack_peek(s)].x,points[stack_peek(s)].y,points[i].x,points[i].y);
        SDL_RenderPresent(renderer);
        stack_push(s,i);
    }
    int* indexes = stack_to_arr(s);
    SDL_Point* convex_hull = malloc(sizeof(SDL_Point) * s->len);
    for (int i = 0; i < s->len;i++){
        convex_hull[i] = points[indexes[i]];
    }
    DrawCircle(renderer,pivot.x,pivot.y,green);
    SDL_SetRenderDrawColor(renderer,white.r,white.g,white.b,white.a);
    SDL_RenderDrawPoints(renderer,points + 1,n - 1);
    SDL_SetRenderDrawColor(renderer,orange.r,orange.g,orange.b,orange.a);
    for (int i = 0; i < s->len - 1;i++){
        DrawCircle(renderer,convex_hull[i].x,convex_hull[i].y,green);
        SDL_RenderDrawLine(renderer,convex_hull[i].x,convex_hull[i].y,convex_hull[i + 1].x,convex_hull[i + 1].y);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
    SDL_RenderDrawLine(renderer,convex_hull[0].x,convex_hull[0].y,convex_hull[s->len - 1].x,convex_hull[s->len - 1].y);
    SDL_RenderPresent(renderer);
    result res = {.arr = convex_hull,.len=s->len};
    free(indexes);
    free_stack(s);
    return res;
}

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n; //number of points
    int xmin,xmax,ymin,ymax; // will be used to determine a bounding box.
    FILE* in_f = fopen(argv[1],"r");
    SDL_Point* points = build_arr(in_f,&n);
    fclose(in_f);
    SDL_Window* window;
    SDL_Renderer* renderer;
    bounding_box(points,n,&xmin,&xmax,&ymin,&ymax);
    const int TEXTURE_W = xmax - xmin + 15;
    const int TEXTURE_H = ymax - ymin + 15;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"Graham scan algorithm, animated by Esteban795.");
    if (status == 1) {
        free(points);
        return EXIT_FAILURE;
    }
    SDL_RenderDrawPoints(renderer,points,n);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    
    //result res = graham_scan(points,n,renderer);
    SDL_Delay(5000);
    

    //destroy part
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if  (window != NULL) SDL_DestroyWindow(window);
    free(points);
    //free(res.arr);
    return EXIT_SUCCESS;
}


//gcc convex_hull.c -o ch -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2_image
