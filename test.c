#include "SDL.h"
#include <SDL2/SDL_image.h>

int main(int argc, char *argv[])
{
int a = 1;
SDL_Event event;

SDL_Window* window = NULL;
SDL_Surface* background_surface = NULL;
SDL_Texture* background_texture = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* button_surface = NULL;
SDL_Texture* button_texture = NULL;

SDL_Rect button_pos;
button_pos.w = 320;
button_pos.h = 65;
button_pos.x = 0;
button_pos.y = 0;

SDL_Init(SDL_INIT_VIDEO);
window = SDL_CreateWindow(
    "Cosmic Racing",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    320,
    568,
    SDL_WINDOW_SHOWN
);
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
background_surface = IMG_Load("map.jpg");
background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);

button_surface = SDL_LoadBMP("th.jpeg");
button_texture = SDL_CreateTextureFromSurface(renderer, button_surface);

while(a)
{
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
    {
        a = 0;
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    SDL_RenderCopy(renderer, button_texture, NULL, &button_pos);
    SDL_RenderPresent(renderer);
}

SDL_DestroyTexture(button_texture);
SDL_FreeSurface(button_surface);
SDL_DestroyRenderer(renderer);
SDL_DestroyTexture(background_texture);
SDL_FreeSurface(background_surface);
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}

//gcc test.c -Wvla -Wall -Wextra -fsanitize=address $(sdl2-config --cflags --libs) -o sdl -libsdl2-