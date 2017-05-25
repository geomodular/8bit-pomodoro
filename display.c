#include <SDL2/SDL.h>
#include "db16.h"
#include "display.h"

static void draw(void* instance, SDL_Renderer* renderer)
{
    SDL_assert(instance != NULL);
    SDL_assert(renderer != NULL);

    struct Display* dsp = instance;

    /* SDL_Color color; */
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {

            int index = j * 5 + i;
            int x = i * 16 + dsp->widget.rect.x;
            int y = j * 16 + dsp->widget.rect.y;

            if (dsp->leds[index] == SDL_TRUE) {
                SDL_RenderCopy(renderer, dsp->texture,
                        &(SDL_Rect) {143, 2, 10, 10},
                        &(SDL_Rect) {x, y, 10, 10});
                /* color = db16_pal[6]; */
            } else {
                SDL_RenderCopy(renderer, dsp->texture,
                        &(SDL_Rect) {143, 14, 10, 10},
                        &(SDL_Rect) {x, y, 10, 10});
                /* color = db16_pal[12]; */
            }

            /* SDL_Rect rect = dsp->widget.rect; */
            /* rect.x = i * 16 + rect.x; */
            /* rect.y = j * 16 + rect.y; */

            /* SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); */
            /* SDL_RenderFillRect(renderer, &rect); */
        }
    }

}

static void click(void* instance) {}

struct Display* display_create(SDL_Point point, SDL_Texture* texture)
{
    SDL_assert(texture != NULL);

    struct Display* dsp = calloc(1, sizeof * dsp);

    if (dsp) {
        dsp->widget.rect.x = point.x;
        dsp->widget.rect.y = point.y;
        dsp->widget.rect.w = 10;
        dsp->widget.rect.h = 10;
        dsp->widget.draw = draw;
        dsp->widget.click = click;
        dsp->texture = texture;
    }

    return dsp;
}

void display_set(struct Display* display, int index, SDL_bool value)
{
    SDL_assert(display != NULL);
    display->leds[index] = value;
}

void display_toggle(struct Display* display, int index)
{
    SDL_assert(display != NULL);
    display->leds[index] = !display->leds[index];
}

void display_clear(struct Display* display)
{
    SDL_assert(display != NULL);

    for (int i = 0; i < 25; i++)
        display->leds[i] = SDL_FALSE;
}

void display_fill(struct Display* display)
{
    SDL_assert(display != NULL);

    for (int i = 0; i < 25; i++)
        display->leds[i] = SDL_TRUE;
}

void display_destroy(struct Display* display)
{
    SDL_assert(display != NULL);
    free(display);
}
