#include <SDL2/SDL.h>
#include "widget.h"


inline void widget_set_draw(void* widget, void (*draw)(void* instance, SDL_Renderer* renderer))
{
    SDL_assert(widget != NULL);
    SDL_assert(draw != NULL);

    struct Widget* w = widget;
    w->draw = draw;
}

inline void widget_set_click(void* widget, widget_click click)
{
    SDL_assert(widget != NULL);
    SDL_assert(click != NULL);

    struct Widget* w = widget;
    w->click = click;
}

inline void widget_enable(void* widget)
{
    SDL_assert(widget != NULL);

    struct Widget* w = widget;
    w->disabled = SDL_FALSE;
}

inline void widget_disable(void* widget)
{
    SDL_assert(widget != NULL);

    struct Widget* w = widget;
    w->disabled = SDL_TRUE;
}
