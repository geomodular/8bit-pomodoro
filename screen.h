#pragma once

#include <SDL2/SDL.h>

void screen_attach(void* widget);
void screen_detach_all();

void screen_draw(SDL_Renderer* renderer);
void screen_mouse_move(int x, int y);
void screen_mouse_down(int x, int y);
void screen_mouse_up(int x, int y);

