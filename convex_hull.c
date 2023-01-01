#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 1280
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
        printf("Point (%d,%d)",points[i].x,points[i].y);
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
point* adapt_coordinates(point* points,int n,int xmin,int ymin,int xmax,int ymax){
    int rect_width = ceil(xmax - xmin); 
    int rect_height = ceil(ymax - ymin);
    int w_ratio = 1 + rect_width / WIDTH;
    int h_ratio = 1 + rect_height / HEIGHT;
    point* resized_points = malloc(sizeof(point) * n);
    for (int i = 0; i < n;i++){
        point p = {.x = points[i].x / w_ratio, .y = points[i].y / h_ratio};
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

////

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n;
    FILE* in_f = fopen(argv[1],"r");
    fscanf(in_f,"%d\n",&n); // number of points
    point* points = build_arr(in_f,n);
    fclose(in_f);
    int xmin,xmax,ymin,ymax;
    container_rect(points,n,&xmin,&xmax,&ymin,&ymax);
    point* resized_points = adapt_coordinates(points,n,xmin,ymin,xmax,ymax);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"Graham scan algorithm, animated by Esteban795.");
    if (status == 1) {
        free(resized_points);
        return EXIT_FAILURE;
    };
    printf("Point 0 : (%d,%d)\n",resized_points[0].x,resized_points[0].y);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    DrawCircle(renderer,resized_points[0].x,resized_points[0].y,10);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    //destroy part
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if  (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
    free(resized_points);
    return EXIT_SUCCESS;
    
}


//gcc convex_hull.c -o ch -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs)
