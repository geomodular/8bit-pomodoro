#include <stdlib.h>

#include <SDL2/SDL.h>

#include "screen.h"
#include "widget.h"

struct Item
{
    void* widget;
    struct Item* next; // TAIL
};

static struct Item* last_widget;

void screen_attach(void* widget)
{
    SDL_assert(widget != NULL);

    struct Item* l = calloc(1, sizeof * l);

    SDL_assert(l != NULL);

    l->widget = widget;

    if (last_widget != NULL)
        l->next = last_widget;
    last_widget = l;
}

void screen_detach_all()
{
    struct Item* actual = last_widget;

    while (actual != NULL) {
        struct Item* next = actual->next;
        free(actual);
        actual = next;
    }
}

void screen_draw(SDL_Renderer* renderer)
{
    struct Item* actual = last_widget;

    while (actual != NULL) {
        struct Widget* w = actual->widget;

        SDL_assert(w != NULL);

        w->draw(w, renderer);
        actual = actual->next;
    }
}

void screen_mouse_move(int x, int y)
{
    struct Item* actual = last_widget;

    while (actual != NULL) {
        struct Widget* w = actual->widget;

        /* Check if mouse is in rect */
        if (!w->disabled) {
            if (SDL_PointInRect(&(SDL_Point) {x, y}, &w->rect) == SDL_TRUE) {
                w->mouse_on = SDL_TRUE;
            } else {
                w->mouse_on = SDL_FALSE;
                w->mouse_down = SDL_FALSE;
            }
        }

        actual = actual->next;
    }
}

void screen_mouse_down(int x, int y)
{
    struct Item* actual = last_widget;

    while (actual != NULL) {
        struct Widget* w = actual->widget;

        /* Check if mouse is in rect */
        if (!w->disabled) {
            if (SDL_PointInRect(&(SDL_Point) {x, y}, &w->rect) == SDL_TRUE) {
                w->mouse_down = SDL_TRUE;
            } else {
                w->mouse_down = SDL_FALSE;
            }
        }

        actual = actual->next;
    }
}

void screen_mouse_up(int x, int y)
{
    struct Item* actual = last_widget;

    while (actual != NULL) {
        struct Widget* w = actual->widget;

        /* Check if mouse is in rect */
        if (!w->disabled) {
            if (SDL_PointInRect(&(SDL_Point) {x, y}, &w->rect) == SDL_TRUE) {
                if (w->mouse_down == SDL_TRUE) {
                    w->mouse_down = SDL_FALSE;
                    w->click(w);
                }
            }
        }

        actual = actual->next;
    }
}
