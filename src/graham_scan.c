#include "convex_hull_gui.h"

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