// gcc main.c -lSDL2 -o main && ./main

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef int32_t i32;
typedef uint32_t u32;

#define GAME_WIDTH 32
#define GAME_HEIGHT 18
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

int main()
{
  bool GAME[GAME_WIDTH][GAME_HEIGHT];
  for (int i = 0; i < GAME_WIDTH; i++) {
    for (int j = 0; j < GAME_HEIGHT; j++) {
      GAME[i][j] = false;
    }
  }
  u32 WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;
  SDL_Window *window = SDL_CreateWindow("Game of Life", 0, 0, 384, 216, WINDOW_FLAGS);
  assert(window);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  i32 running = 1;
  i32 full_screen = 0;
  while (running) {
    int win_width, win_height;
    SDL_GetWindowSize(window, &win_width, &win_height);
    int cell_size = min(win_width / GAME_WIDTH, win_height / GAME_HEIGHT);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            running = 0;
            break;
          case 'f':
            full_screen = !full_screen;
            if (full_screen) {
              SDL_SetWindowFullscreen(window, WINDOW_FLAGS | SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else {
              SDL_SetWindowFullscreen(window, WINDOW_FLAGS);
            }
            break;
          default:
            break;
        }
      } else if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    int mouse_x, mouse_y;
    int mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_x = mouse_x / cell_size;
    mouse_y = mouse_y / cell_size;
    if (mouse_state & SDL_MOUSEBUTTONDOWN && mouse_x < GAME_WIDTH && mouse_y < GAME_HEIGHT) {
      GAME[mouse_x][mouse_y] = true;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectangle;
    for (int i = 0; i < GAME_WIDTH; i++) {
      for (int j = 0; j < GAME_HEIGHT; j++) {
        if (GAME[i][j]) {
          rectangle.x = cell_size * i;
          rectangle.y = cell_size * j;
          rectangle.w = cell_size;
          rectangle.h = cell_size;
          SDL_RenderFillRect(renderer, &rectangle);
        }
      }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
  }
  SDL_DestroyWindow(window);
  return 0;
}
