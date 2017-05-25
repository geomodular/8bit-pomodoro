#pragma once

#include <SDL2/SDL.h>
#include "widget.h"

struct Button
{
    struct Widget widget;
    SDL_Texture* texture;
};

enum ButtonEvent {
    PLAY_EVENT = SDL_USEREVENT,
    PAUSE_EVENT,
    STOP_EVENT,
    LAUNCH_EVENT
};

struct Button* button_play_create(SDL_Point point, SDL_Texture* texture);
struct Button* button_stop_create(SDL_Point point, SDL_Texture* texture);
struct Button* button_pause_create(SDL_Point point, SDL_Texture* texture);
struct Button* button_launch_create(SDL_Point point, SDL_Texture* texture);
void button_destroy(struct Button* button);

