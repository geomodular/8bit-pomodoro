#include <SDL2/SDL.h>

#include "buttons.h"

static void draw_play(void* instance, SDL_Renderer* renderer)
{
    SDL_assert(instance != NULL);
    SDL_assert(renderer != NULL);

    struct Button* btn = instance;

    if (btn->widget.disabled == SDL_TRUE) {
        SDL_RenderCopy(renderer, btn->texture,
                &(SDL_Rect) {28, 84, 26, 26}, &btn->widget.rect);
    } else {
        if (btn->widget.mouse_down == SDL_FALSE) {
            if (btn->widget.mouse_on == SDL_FALSE) {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {28, 0, 26, 26}, &btn->widget.rect);
            } else {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {28, 28, 26, 26}, &btn->widget.rect);
            }
        } else {
            SDL_RenderCopy(renderer, btn->texture,
                    &(SDL_Rect) {28, 56, 26, 26}, &btn->widget.rect);
        }
    }
}

static void draw_stop(void* instance, SDL_Renderer* renderer)
{
    SDL_assert(instance != NULL);
    SDL_assert(renderer != NULL);

    struct Button* btn = instance;

    if (btn->widget.disabled == SDL_TRUE) {
        SDL_RenderCopy(renderer, btn->texture,
                &(SDL_Rect) {56, 84, 26, 26}, &btn->widget.rect);
    } else {
        if (btn->widget.mouse_down == SDL_FALSE) {
            if (btn->widget.mouse_on == SDL_FALSE) {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {56, 0, 26, 26}, &btn->widget.rect);
            } else {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {56, 28, 26, 26}, &btn->widget.rect);
            }
        } else {
            SDL_RenderCopy(renderer, btn->texture,
                    &(SDL_Rect) {56, 56, 26, 26}, &btn->widget.rect);
        }
    }
}

static void draw_pause(void* instance, SDL_Renderer* renderer)
{
    SDL_assert(instance != NULL);
    SDL_assert(renderer != NULL);

    struct Button* btn = instance;

    if (btn->widget.disabled == SDL_TRUE) {
        SDL_RenderCopy(renderer, btn->texture,
                &(SDL_Rect) {0, 84, 26, 26}, &btn->widget.rect);
    } else {
        if (btn->widget.mouse_down == SDL_FALSE) {
            if (btn->widget.mouse_on == SDL_FALSE) {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {0, 0, 26, 26}, &btn->widget.rect);
            } else {
                SDL_RenderCopy(renderer, btn->texture,
                        &(SDL_Rect) {0, 28, 26, 26}, &btn->widget.rect);
            }
        } else {
            SDL_RenderCopy(renderer, btn->texture,
                    &(SDL_Rect) {0, 56, 26, 26}, &btn->widget.rect);
        }
    }
}

static void draw_launch(void* instance, SDL_Renderer* renderer)
{
    SDL_assert(instance != NULL);
    SDL_assert(renderer != NULL);

    struct Button* btn = instance;

    if (btn->widget.mouse_down == SDL_FALSE) {
        if (btn->widget.mouse_on == SDL_FALSE) {
            SDL_RenderCopy(renderer, btn->texture,
                &(SDL_Rect) {84, 0, 57, 26}, &btn->widget.rect);
        } else {
            SDL_RenderCopy(renderer, btn->texture,
                &(SDL_Rect) {84, 28, 57, 26}, &btn->widget.rect);
        }
    } else {
        SDL_RenderCopy(renderer, btn->texture,
            &(SDL_Rect) {84, 56, 57, 26}, &btn->widget.rect);
    }
}

static void click_play(void* instance)
{
    SDL_Event event;
    event.type = PLAY_EVENT;
    SDL_PushEvent(&event);
}

static void click_stop(void* instance)
{
    SDL_Event event;
    event.type = STOP_EVENT;
    SDL_PushEvent(&event);
}

static void click_pause(void* instance)
{
    SDL_Event event;
    event.type = PAUSE_EVENT;
    SDL_PushEvent(&event);
}

static void click_launch(void* instance)
{
    SDL_Event event;
    event.type = LAUNCH_EVENT;
    SDL_PushEvent(&event);
}

struct Button* button_play_create(SDL_Point point, SDL_Texture* texture)
{
    SDL_assert(texture != NULL);

    struct Button* btn = calloc(1, sizeof * btn);

    if (btn) {
        btn->widget.rect.x = point.x;
        btn->widget.rect.y = point.y;
        btn->widget.rect.w = 26;
        btn->widget.rect.h = 26;
        btn->widget.draw = draw_play;
        btn->widget.click = click_play;
        btn->texture = texture;
    }

    return btn;
}

struct Button* button_stop_create(SDL_Point point, SDL_Texture* texture)
{
    SDL_assert(texture != NULL);

    struct Button* btn = calloc(1, sizeof * btn);

    if (btn) {
        btn->widget.rect.x = point.x;
        btn->widget.rect.y = point.y;
        btn->widget.rect.w = 26;
        btn->widget.rect.h = 26;
        btn->widget.draw = draw_stop;
        btn->widget.click = click_stop;
        btn->texture = texture;
    }

    return btn;
}

struct Button* button_pause_create(SDL_Point point, SDL_Texture* texture)
{
    SDL_assert(texture != NULL);

    struct Button* btn = calloc(1, sizeof * btn);

    if (btn) {
        btn->widget.rect.x = point.x;
        btn->widget.rect.y = point.y;
        btn->widget.rect.w = 26;
        btn->widget.rect.h = 26;
        btn->widget.draw = draw_pause;
        btn->widget.click = click_pause;
        btn->texture = texture;
    }

    return btn;
}

struct Button* button_launch_create(SDL_Point point, SDL_Texture* texture)
{
    SDL_assert(texture != NULL);

    struct Button* btn = calloc(1, sizeof * btn);

    if (btn) {
        btn->widget.rect.x = point.x;
        btn->widget.rect.y = point.y;
        btn->widget.rect.w = 57;
        btn->widget.rect.h = 26;
        btn->widget.draw = draw_launch;
        btn->widget.click = click_launch;
        btn->texture = texture;
    }

    return btn;
}

void button_destroy(struct Button* button)
{
    SDL_assert(button != NULL);
    free(button);
}

