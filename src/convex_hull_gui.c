#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include <pthread.h>


SDL_Point pivot;

#define WIDTH 800
#define HEIGHT 800
#define RADIUS 3 


struct Camera {
    SDL_Rect source;
    int current_scale;
};

typedef struct Camera camera;

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}

struct thread_args {
    SDL_Renderer* renderer;
    SDL_Point* points;
    int n;
    int TEXTURE_W;
    int TEXTURE_H;
};

typedef struct thread_args thread_args;

typedef int T;

struct node {
    T data;
    struct node* next;
};

typedef struct node node;

struct custom_stack {
    int len;
    node* top;
};

typedef struct custom_stack custom_stack;



node* new_node(T d){
    node* n = malloc(sizeof(node));
    n->data = d;
    n->next = NULL;
    return n;
}

custom_stack* stack_new(void){
    custom_stack* s = malloc(sizeof(custom_stack));
    s->len = 0;
    s->top = NULL;
    return s;
}

void free_list(node* n){
    if (n == NULL) return;
    free_list(n->next);
    free(n);
}

void free_stack(custom_stack* s){
    free_list(s->top);
    free(s);
}

void stack_push(custom_stack* s,T data){
    node* n = new_node(data);
    if (s->len > 0){
        node* previous_top = s->top;
        n->next = previous_top;
    }
    s->len++;
    s->top = n;
}

T stack_pop(custom_stack* s){
    assert(s->len > 0);
    node* top = s->top;
    T result = top->data;
    s->top = top->next;
    s->len = s->len - 1;
    free(top);
    return result;
}

T stack_peek(custom_stack* s){
    assert(s->len >0);
    return s->top->data;
}

T stack_peek_second(custom_stack* s){
    assert(s->len > 1);
    return s->top->next->data;
}

void add_rec(custom_stack* s,node* n){
    if (n == NULL) return;
    add_rec(s,n->next);
    stack_push(s,n->data);
}

custom_stack* stack_copy(custom_stack* s){
    custom_stack* new_stack = stack_new();
    add_rec(new_stack,s->top);
    return new_stack;
}

void print_rec(node* n){
    if (n != NULL){
        printf("Data : %d\n",n->data);
        print_rec(n->next);
    }
}

void stack_print(custom_stack* s){
    print_rec(s->top);
}

T* stack_to_arr(custom_stack* s){
    T* arr = malloc(sizeof(int) * s->len);
    custom_stack* c = stack_copy(s);
    for (int i = 0; i < s->len;i++){
        arr[i] = stack_pop(c);
    }
    free_stack(c);
    return arr;
}



SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {20, 20, 200, 255};
SDL_Color red = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};


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
Modify points array to keep only points that form a unique angle with pivot
*/
int ignore_colinear_points(SDL_Point* points,int n){
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
    DrawCircle(renderer,minimum.x,minimum.y,radius);
    SDL_RenderPresent(renderer);
    return index;
}





int apply_effect_on_condition(custom_stack* s,SDL_Renderer* renderer,SDL_Point* points,int i){
    SDL_Point top = points[stack_peek(s)];
    SDL_Point next_to_top = points[stack_peek_second(s)];
    SDL_SetRenderDrawColor(renderer,blue.r,blue.g,blue.b,blue.a);
    SDL_RenderDrawLine(renderer,top.x,top.y,points[i].x,points[i].y);
    SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
    SDL_RenderPresent(renderer);
    SDL_Delay(200);
    int o = orientation(next_to_top,top,points[i]);
    if (o != 2){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_Delay(25);
        SDL_RenderDrawLine(renderer,top.x,top.y,next_to_top.x,next_to_top.y);
        SDL_RenderDrawLine(renderer,points[i].x,points[i].y,top.x,top.y);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
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

void graham_scan(SDL_Renderer* renderer,SDL_Point* points,int n){
    int piv = find_pivot(points,n,renderer,RADIUS);
    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for (int i = 0; i < n;i++){
        DrawCircle(renderer,points[i].x,points[i].y,5);
    }
    SDL_RenderPresent(renderer);
    pivot = points[piv];
    swap(points,0,piv);
    qsort(points,n,sizeof(SDL_Point),compare_qsort);
    //We now need to remove points that have the same angle, and only keep the farthest from pivot ones.
    int new_len = ignore_colinear_points(points,n);
    custom_stack* s = stack_new();
    SDL_Delay(1000);
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
    DrawCircle(renderer,pivot.x,pivot.y,RADIUS);
    SDL_SetRenderDrawColor(renderer,white.r,white.g,white.b,white.a);
    SDL_RenderDrawPoints(renderer,points + 1,n - 1);
    SDL_SetRenderDrawColor(renderer,orange.r,orange.g,orange.b,orange.a);
    for (int i = 0; i < s->len - 1;i++){
        DrawCircle(renderer,convex_hull[i].x,convex_hull[i].y,RADIUS);
        SDL_RenderDrawLine(renderer,convex_hull[i].x,convex_hull[i].y,convex_hull[i + 1].x,convex_hull[i + 1].y);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
    SDL_RenderDrawLine(renderer,convex_hull[0].x,convex_hull[0].y,convex_hull[s->len - 1].x,convex_hull[s->len - 1].y);
    SDL_RenderPresent(renderer);
    free(indexes);
    free_stack(s);
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

void print_cam(camera cam){
    printf("Camera : x : %d, y : %d,width : %d, height %d, current_scale : %d\n",cam.source.x,cam.source.y,cam.source.w,cam.source.h,cam.current_scale);
}

void main_loop(void* args){
    thread_args data = *(thread_args*) args;
    SDL_Point* points = data.points;
    int n = data.n;
    SDL_Renderer* renderer = data.renderer;
    int TEXTURE_W = data.TEXTURE_W;
    int TEXTURE_H = data.TEXTURE_H;
    SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,TEXTURE_W + 100,TEXTURE_H + 100);
    camera cam = {.source = {WIDTH/4,WIDTH/4,WIDTH/16,HEIGHT/16}, .current_scale = 16};
    SDL_Rect dest = {10,10,WIDTH - 20,HEIGHT- 20};
    SDL_Event e;
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_RenderDrawPoints(renderer,points,n);
    SDL_RenderCopy(renderer,texture,&cam.source,&dest);
    SDL_RenderPresent(renderer);
    int running = 1;
    while (running){        
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    print_cam(cam);
                    if (cam.source.y < -2) break;
                    cam.source.y -= 3;
                    break;
                case SDLK_DOWN:
                    print_cam(cam);
                    if (cam.source.y > cam.source.h * cam.current_scale) break;
                    cam.source.y += 3;
                    break;
                case SDLK_LEFT:
                    print_cam(cam);
                    if (cam.source.x < -2) break;
                    cam.source.x -= 5;
                    break;
                case SDLK_RIGHT:
                    print_cam(cam);
                    printf("\n");
                    if (cam.source.x > cam.source.w * cam.current_scale) break;
                    cam.source.x += 5;
                    break;
                case SDLK_1:
                    if (cam.current_scale <= 2) break;
                    cam.source.w *= 2;
                    cam.source.h *= 2;
                    cam.current_scale /= 2;
                    break;
                case SDLK_2:
                    if (cam.current_scale > 32) break;
                    cam.source.w /= 2;
                    cam.source.h /= 2;
                    cam.current_scale *= 2;
                    break;
                default:
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderDrawPoints(renderer,points,1000);

        SDL_SetRenderTarget(renderer,NULL);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderCopy(renderer,texture,&cam.source,&dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_DestroyTexture(texture);
    return NULL;
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
    graham_scan(renderer,points,n);

    SDL_Delay(200);
    //destroy part
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if  (window != NULL) SDL_DestroyWindow(window);
    free(points);
    return EXIT_SUCCESS;
}


//gcc convex_hull_gui.c -o chgui -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags --libs) -lSDL2_image
