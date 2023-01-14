#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "utilities/stack.c"
#include "SDL_utilities/color_related.c"
#include "utilities/array_manipulation.c"
#include "convex_hull_gui.h"

#define WIDTH 1500
#define HEIGHT 1000
#define RADIUS 3 

/*
Animation of how Graham Scan algorithm actually works behind the scenes.
Make sure the file you're passing as an argument has the following pattern : 

How to use it : 
./whateveryoucompileto filepath


n (which is the number of points to consider)
x1 y1
x2 y2
x3 y3 
.
.
.
xn yn

Not respecting this format will result in undefined behaviour. (because fscanf..)
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
