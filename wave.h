#pragma once

#include <SDL2/SDL.h>

// https://gist.github.com/armornick/3447121
// https://github.com/jakebesworth/Simple-SDL2-Audio/blob/master/src/audio.c

int wave_init();
void wave_play(const char* filename);
void wave_end();

