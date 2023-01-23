#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include <stdbool.h>



#include "utilities/custom_stack.h"
#include "utilities/array_manipulation.h"
#include "SDL_utilities/drawers.h"
#include "utilities/geometry.h"



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

Not following this format will result in undefined behaviour. (because fscanf..)
*/


SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};


int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int apply_effect_on_condition(custom_stack* s,SDL_Renderer* renderer,SDL_Point* points,int i,SDL_Texture* texture,camera* cam,SDL_Rect dest){
    SDL_Point top = points[stack_peek(s)];
    SDL_Point next_to_top = points[stack_peek_second(s)];

    //Drawing a line between the three possible points that belongs to convex hull.
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,blue.r,blue.g,blue.b,blue.a);
    SDL_RenderDrawLine(renderer,top.x,top.y,points[i].x,points[i].y);
    SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
    update_screen(renderer,texture,cam,dest);

    poll_events(renderer,texture,cam,dest);
    SDL_Delay(100); //so people can actually watch what happens lol.
    int o = orientation(next_to_top,top,points[i]); //Going counterclockwise => top isn't in convex hull.
    if (o != 2){
        SDL_SetRenderTarget(renderer,texture);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
        SDL_RenderDrawLine(renderer,points[i].x,points[i].y,top.x,top.y);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderDrawPoint(renderer,points[i].x,points[i].y);
        SDL_RenderDrawPoint(renderer,top.x,top.y);
        SDL_RenderDrawPoint(renderer,next_to_top.x,next_to_top.y);
        poll_events(renderer,texture,cam,dest);
    }
    return o;
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

void graham_scan(SDL_Renderer* renderer,int TEXTURE_W,int TEXTURE_H,SDL_Point* points,int n){
    SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,TEXTURE_W + 100,TEXTURE_H + 100);
    camera cam = {.source = {WIDTH/4,WIDTH/4,WIDTH/4,HEIGHT/4}, .current_scale = 4}; //initial zoom at 4 scale, centered camera.
    SDL_Rect dest = {10,10,WIDTH - 20,HEIGHT- 20};
    int en_cours = 1;
    SDL_Event e;
    //Setup the window : white background
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    //Drawing black points on the texture.
    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    SDL_RenderDrawPoints(renderer,points,n);

    SDL_SetRenderTarget(renderer,NULL);
    SDL_RenderCopy(renderer,texture,&(cam.source),&dest);
    SDL_RenderPresent(renderer);

    //Starting Graham's scan algorithm.
    int piv = find_pivot(points,n,renderer,&cam,texture,dest);
    pivot = points[piv];
    swap(points,0,piv);


    //Sorting using angle formed by pivot, point A and B. The smaller the angle, the earlier they appear in the sorted array.
    qsort(points,n,sizeof(SDL_Point),compare_qsort);

    //Removing points that are colinear.Only keeps the farthest from pivot if there are colinear points.
    SDL_SetRenderTarget(renderer,texture);
    int new_len = ignore_colinear_points(renderer,points,n);
    update_screen(renderer,texture,&cam,dest);
    custom_stack* s = stack_new();
    stack_push(s,0);
    stack_push(s,1);
    stack_push(s,2);
    SDL_Delay(500);
    for (int i = 3; i < new_len;i++){
        while (s->len > 1 && apply_effect_on_condition(s,renderer,points,i,texture,&cam,dest) != 2) {
            stack_pop(s);
        }
        SDL_SetRenderTarget(renderer,texture);
        SDL_RenderDrawLine(renderer,points[stack_peek(s)].x,points[stack_peek(s)].y,points[i].x,points[i].y);
        SDL_SetRenderTarget(renderer,NULL);
        SDL_RenderPresent(renderer);
        stack_push(s,i);
    }

    //Indexes of points in points that belongs to convex hull.
    int* indexes = stack_to_arr(s);

    //Making a cleaner array to store the convex hull points.
    SDL_Point* convex_hull = malloc(sizeof(SDL_Point) * s->len);
    for (int i = 0; i < s->len;i++){
        convex_hull[i] = points[indexes[i]];
    }
    free(indexes);
    
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,green.r,green.g,green.b,green.a);

    //Draws circle on points that belongs to the convex hull.
    for (int i = 0; i < s->len - 1;i++){
        SDL_SetRenderTarget(renderer,texture);
        DrawCircle(renderer,convex_hull[i].x,convex_hull[i].y,RADIUS);
        SDL_RenderDrawLine(renderer,convex_hull[i].x,convex_hull[i].y,convex_hull[i + 1].x,convex_hull[i + 1].y);
        poll_events(renderer,texture,&cam,dest);
        SDL_Delay(100);
    }
    SDL_SetRenderTarget(renderer,texture);
    SDL_RenderDrawLine(renderer,convex_hull[0].x,convex_hull[0].y,convex_hull[s->len - 1].x,convex_hull[s->len - 1].y); //last line to close the hull.
    update_screen(renderer,texture,&cam,dest);
    stack_free(s);
    free(convex_hull);
    while (en_cours){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        en_cours = 0;
                        break;
                    default:
                        break;
                }
                poll_events(renderer,texture,&cam,dest);
            }
        }
    }
    SDL_DestroyTexture(texture);
}



int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n; //number of points
    FILE* in_f = fopen(argv[1],"r");
    SDL_Point* points = build_arr(in_f,&n);
    fclose(in_f);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int TEXTURE_W;
    int TEXTURE_H;
    bounding_box(points,n,&TEXTURE_W,&TEXTURE_H);
    int status = start_SDL(&window,&renderer,WIDTH,HEIGHT,"Graham scan algorithm, animated by Esteban795.");
    if (status == 1) {
        free(points);
        return EXIT_FAILURE;
    }
    graham_scan(renderer,TEXTURE_W,TEXTURE_H,points,n);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if  (window != NULL) SDL_DestroyWindow(window);
    free(points);
    return EXIT_SUCCESS;
}

//gcc convex_hull_gui.c ./utilities/custom_stack.c ./utilities/array_manipulation.c ./SDL_utilities/drawers.c ./utilities/geometry.c -o chgui -Wvla -Wall -Wextra -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2