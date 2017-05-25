#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "db16.h"
#include "screen.h"
#include "buttons.h"
#include "display.h"
#include "wave.h"
#include "widget.h"

#define INTERVAL 60000

enum State {
    IDLE,
    RUNNING,
    DONE,
    PAUSE
};

struct AppState {
    enum State state;
    unsigned long t_last;
    unsigned long t_actual;
    int pixel; // actual pixel
    SDL_bool running;

    /* Scope transfer */
    SDL_Renderer* renderer;
    struct Button* play_button;
    struct Button* pause_button;
    struct Button* stop_button;
    struct Display* display;
};

Uint32 t_last;

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename);
void update(void* appState);

int main(int argc, char* argv[])
{
    struct AppState app_state = { .running = SDL_TRUE };

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (wave_init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not open audio: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Pomodoro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 240, SDL_WINDOW_SHOWN);
    SDL_assert(window != NULL);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE|SDL_RENDERER_SOFTWARE);
    SDL_assert(renderer != NULL);

    /* Play wav */
    wave_play("start.wav");

    /* Load textures */

    SDL_Texture* texture_gfx = load_texture(renderer, "gfx.bmp");
    SDL_assert(texture_gfx != NULL);

    /* Create and attach leds */

    struct Display* display = display_create((SDL_Point) {120, 50}, texture_gfx);
    struct Button* play_button = button_play_create((SDL_Point) {116, 160}, texture_gfx);
    struct Button* pause_button = button_pause_create((SDL_Point) {144, 160}, texture_gfx);
    struct Button* stop_button = button_stop_create((SDL_Point) {172, 160}, texture_gfx);
    /* struct Button* launch_button = button_launch_create((SDL_Point) {120, 190}, texture_gfx); */

    widget_disable(stop_button);
    widget_disable(pause_button);

    screen_attach(display);
    screen_attach(play_button);
    screen_attach(stop_button);
    screen_attach(pause_button);
    /* screen_attach(launch_button); */

    app_state.display = display;
    app_state.renderer = renderer;
    app_state.play_button = play_button;
    app_state.pause_button = pause_button;
    app_state.stop_button = stop_button;

    t_last = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(update, &app_state, -1, 1);
#else
    while (app_state.running) {
        update(&app_state);
    }
#endif

    screen_detach_all();
    button_destroy(play_button);
    button_destroy(stop_button);
    button_destroy(pause_button);
    /* button_destroy(launch_button); */
    display_destroy(display);

    SDL_DestroyTexture(texture_gfx);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    wave_end();
    SDL_Quit();

    return EXIT_SUCCESS;
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename)
{
    SDL_assert(renderer != NULL);
    SDL_assert(filename != NULL);

    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (surface == NULL) return NULL;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void update(void* appState)
{

    struct AppState* app_state = appState;

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            app_state->running = SDL_FALSE;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
        } else if (e.type == SDL_MOUSEMOTION) {
            screen_mouse_move(e.motion.x, e.motion.y);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            screen_mouse_down(e.button.x, e.button.y);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            screen_mouse_up(e.button.x, e.button.y);
        } else if (e.type == PAUSE_EVENT) {
            if (app_state->state == RUNNING) {
                app_state->state = PAUSE;

                widget_enable(app_state->play_button);
                widget_enable(app_state->stop_button);
                widget_disable(app_state->pause_button);

                wave_play("click.wav");
            }
        } else if (e.type == STOP_EVENT) {
            if (app_state->state == PAUSE || app_state->state == DONE) {
                app_state->t_last = 0;
                app_state->t_actual = 0;
                app_state->pixel = 0;
                app_state->state = IDLE;

                widget_enable(app_state->play_button);
                widget_disable(app_state->stop_button);
                widget_disable(app_state->pause_button);

                display_clear(app_state->display);

                wave_play("click.wav");
            }
        } else if (e.type == PLAY_EVENT) {
            if (app_state->state == IDLE || app_state->state == PAUSE) {
                app_state->state = RUNNING;

                widget_disable(app_state->play_button);
                widget_disable(app_state->stop_button);
                widget_enable(app_state->pause_button);

                wave_play("click.wav");
            }
        }
    }

    /* Draw screen */

    SDL_SetRenderDrawColor(app_state->renderer, db16_pal[15].r, db16_pal[15].g, db16_pal[15].b, db16_pal[15].a);
    SDL_RenderClear(app_state->renderer);

    SDL_SetRenderDrawColor(app_state->renderer, db16_pal[14].r, db16_pal[14].g, db16_pal[14].b, db16_pal[14].a);
    SDL_RenderFillRect(app_state->renderer, &(SDL_Rect) {0, 160, 320, 2});
    SDL_RenderFillRect(app_state->renderer, &(SDL_Rect) {0, 164, 320, 18});
    SDL_RenderFillRect(app_state->renderer, &(SDL_Rect) {0, 184, 320, 2});

    screen_draw(app_state->renderer);

    /* State update */

    Uint32 t_actual = SDL_GetTicks();
    Uint32 t_dt = t_actual - t_last;
    t_last = t_actual;

    if (app_state->state == RUNNING) {
        app_state->t_actual += t_dt;

        /* New pixel */
        if (app_state->t_actual - app_state->t_last > INTERVAL) {
            display_set(app_state->display, app_state->pixel, SDL_TRUE);
            app_state->pixel++;
            app_state->t_last = app_state->t_actual;

            if (app_state->pixel == 25) {
                app_state->state = DONE;
                widget_disable(app_state->play_button);
                widget_enable(app_state->stop_button);
                widget_disable(app_state->pause_button);
            }
        }
    }

    if (app_state->state != IDLE && app_state->state != DONE) {
        if (app_state->state != PAUSE) {
            if (t_actual % 2000 > 1000) {
                display_set(app_state->display, app_state->pixel, SDL_TRUE);
            } else {
                display_set(app_state->display, app_state->pixel, SDL_FALSE);
            }
        } else {
            display_set(app_state->display, app_state->pixel, SDL_FALSE);
        }
    }

    if (app_state->state == DONE) {
        if (t_actual % 1000 > 500) {
            display_clear(app_state->display);
        } else {
            display_fill(app_state->display);
        }
    }

    /*              */

    SDL_RenderPresent(app_state->renderer);

#ifndef __EMSCRIPTEN__
    SDL_Delay(10);
#endif
}

