#pragma once

#include <SDL2/SDL.h>

typedef void (*widget_draw)(void* instance, SDL_Renderer* renderer);
typedef void (*widget_click)(void* instance);

struct Widget {
    SDL_Rect rect;
    SDL_bool mouse_on;
    SDL_bool mouse_down;
    SDL_bool disabled;

    /* Methods */
    widget_draw draw;
    widget_click click;
};

void widget_set_draw(void* widget, widget_draw draw);
void widget_set_click(void* widget, widget_click click);
void widget_enable(void* widget);
void widget_disable(void* widget);


