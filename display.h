#pragma once

#include <SDL2/SDL.h>
#include "widget.h"

struct Display {
    struct Widget widget;
    SDL_bool leds[25];
    SDL_Texture* texture;
};

struct Display* display_create(SDL_Point point, SDL_Texture* texture);
void display_set(struct Display* display, int index, SDL_bool value);
void display_toggle(struct Display* display, int index);
void display_clear(struct Display* display);
void display_fill(struct Display* display);
void display_destroy(struct Display* display);


