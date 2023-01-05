#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "stack.c"

#define WIDTH 1100
#define HEIGHT 700

struct point {
    int x;  
    int y;
};

typedef struct point point;

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


SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

int max(int a,int b){
    if (a > b) return a;
    return b;
}

int min(int a,int b){
    if (a < b) return a;
    return b;
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

point* copy_points_arr(point* points,int n){
    point* copy_arr = malloc(sizeof(point) * n);
    for (int  i = 0;i < n;i++){
        point p = {.x = points[i].x,.y = points[i].y};
        copy_arr[i] = p;
    }
    return copy_arr;
}

void print_points_arr(point* points, int n){
    for (int i = 0; i < n;i++){
        printf("Point (%d,%d)\n",points[i].x,points[i].y);
    }
}
/////


///// points preprocessing
/*
Gives xmin,ymin, xmax,ymax coordinates of a bounding rectangle (generally not the minimal one)
*/
void container_rect(point* points,int n,int* xmin,int* xmax,int* ymin,int* ymax){
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

/*
Points have no specific reasons to fall under the 1280x720 resolution used here.
So we gotta modify those coordinates to make sure they fall under the size of the window.
*/
point* adapt_coordinates(point* points,int n,int xmin,int ymin,int xmax,int ymax,int* radius){
    int rect_width = ceil(xmax - xmin); 
    int rect_height = ceil(ymax - ymin);
    int w_ratio = rect_width / WIDTH;
    int h_ratio = rect_height / HEIGHT;
    int final_ratio = max(w_ratio,h_ratio) == 0 ? 2 : 1 + max(w_ratio,h_ratio);
    point* resized_points = malloc(sizeof(point) * n);
    *radius = max(5,max(w_ratio,h_ratio));
    for (int i = 0; i < n;i++){
        point p = {.x = points[i].x / final_ratio, .y = points[i].y/ final_ratio};
        resized_points[i] = p;
    }

    free(points);
    return resized_points;
}

point* build_arr(FILE* f,int n){
    point* points = malloc(n * sizeof(point));
    int temp_x,temp_y;
    for (int i = 0; i < n;i++){
        fscanf(f,"%d %d\n",&temp_x,&temp_y);
        point p = {.x = temp_x,.y = temp_y};
        points[i] = p;
    }
    return points;
}
////


/// drawers/

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius,SDL_Color color){
   const int32_t diameter = (radius * 2);
   int32_t x = (radius - 1);
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

void draw_points(SDL_Renderer* renderer,SDL_Color color,point* points,int n,int radius){
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    for (int i = 0; i < n;i++){
        DrawCircle(renderer,points[i].x,points[i].y,radius,white);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
}

point pivot;


struct result {
    point* arr;
    int len;
};

typedef struct result result;

void swap(point* points,int i, int j){
    point temp = points[i];
    points[i] = points[j];
    points[j] = temp;
}

/* Find the orientation between 3 points.
# 0 --> A, B and C are collinear
# 1 --> Clockwise
# 2 --> Counterclockwise
*/
int orientation(point A,point B, point C){
    int res = ((B.y - A.y) * (C.x - B.x) - (B.x - A.x) * (C.y - B.y));
    if (res == 0) return 0;
    if (res > 0) return 2;
    if (res < 0) return 1;
}

int dist(point A,point B){
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y)* (A.y - B.y);
}

/*
Sorts an array of points with respect to the first point.
*/
int compare(const void* A,const void* B){ 
    point a = *(point*)A;
    point b = *(point*)B;
    int o = orientation(pivot,a,b);
    if (o == 0){ //colinear
        if (dist(pivot,b) >= dist(pivot,a)) return -1;
        return 1;
    }
    if (o == 2) return -1;
    if (o == 1) return 1;
}


bool comp(point A,point B){//lexical order with (y,x) coordinates
    if (A.y < B.y) {
        return false;
    } else if (A.y == B.y && B.x > A.x){
        return false;
    }
    return true;
}

/*
Searches for point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(point* points,int n,SDL_Renderer* renderer,int radius){ //lexical order with (y,x) ok
    int index = 0;
    point minimum = points[0];
    for (int i = 0; i < n;i++){
        DrawCircle(renderer,points[i].x,points[i].y,radius,blue);
        if (points[i].y < minimum.y || (points[i].y == minimum.y && points[i].x < minimum.y)){
            SDL_Delay(200);
            DrawCircle(renderer,minimum.x,minimum.y,radius,white);
            index = i;
            minimum = points[i];
            DrawCircle(renderer,minimum.x,minimum.y,radius,green);
        }
        DrawCircle(renderer,points[i].x,points[i].y,radius,white);
    }
    return index;
}


////

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n;
    FILE* in_f = fopen(argv[1],"r");
    fscanf(in_f,"%d\n",&n); // number of points
    point* points = build_arr(in_f,n);
    fclose(in_f);
    int xmin,xmax,ymin,ymax;
    int radius = 10;
    container_rect(points,n,&xmin,&xmax,&ymin,&ymax);
    //point* resized_points = adapt_coordinates(points,n,xmin,ymin,xmax,ymax,&radius);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"Graham scan algorithm, animated by Esteban795.");
    if (status == 1) {
        free(points);
        return EXIT_FAILURE;
    };
    //printf("Point 0 : (%d,%d)\n",resized_points[0].x,resized_points[0].y);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    draw_points(renderer,white,points,n,radius);

    int pivot = find_pivot(points,n,renderer,radius);
    printf("Pivot at (%d,%d)\n",points[pivot].x,points[pivot].y);
    SDL_Delay(10000);
    
    //result test = graham_scan(resized_points,n,radius,renderer);
    //SDL_Delay(10000);

    //destroy part
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if  (window != NULL) SDL_DestroyWindow(window);
    free(points);
    return EXIT_SUCCESS;
}


//gcc convex_hull.c -o ch -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs)
