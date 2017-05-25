#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "wave.h"

struct Wave
{
    const char* filename;

    SDL_AudioSpec spec;

    Uint32 length;
    Uint8* buffer;

    Uint32 actual_length;
    Uint8* actual_buffer;

    struct Wave* next;
};

static struct WaveInternals
{
    SDL_AudioSpec want;
    SDL_AudioDeviceID dev;

    struct Wave first_wave;
} audio;

static void audio_callback(void* userdata, Uint8* stream, int len)
{
    SDL_memset(stream, 0, len);

    struct Wave* wave = userdata;
    struct Wave* actual = wave->next;
    while (actual != NULL) {


        if (actual->actual_length != 0) {
            int len_ = (len > actual->actual_length ? actual->actual_length : len);
            /* printf("%s\n", actual->filename); */

            SDL_MixAudioFormat(stream, actual->actual_buffer, actual->spec.format, len_, SDL_MIX_MAXVOLUME / 2);

            actual->actual_buffer += len_;
            actual->actual_length -= len_;
        }

        actual = actual->next;
    }
}

int wave_init()
{
    audio.want.freq = 44100;
    audio.want.format = AUDIO_U16;
    audio.want.channels = 2;
    audio.want.samples = 4096;
    audio.want.callback = audio_callback;
    audio.want.userdata = &audio.first_wave;
    audio.dev = SDL_OpenAudioDevice(NULL, 0, &audio.want, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audio.dev != 0)
        SDL_PauseAudioDevice(audio.dev, 0);

    return audio.dev;
}

void wave_end()
{
    SDL_PauseAudioDevice(audio.dev, 1);
    SDL_CloseAudioDevice(audio.dev);

    struct Wave* temp = NULL;
    struct Wave* actual = audio.first_wave.next;
    while (actual != NULL) {
        temp = actual;
        actual = actual->next;
        SDL_FreeWAV(temp->buffer);
        free(temp);
    }
}


void wave_play(const char* filename)
{
    SDL_assert(filename != NULL);

    /* Try to search for that file in list */
    struct Wave* wave = NULL;
    struct Wave* last = &audio.first_wave;
    struct Wave* actual = audio.first_wave.next;
    while (actual != NULL) {

        if (strcmp(actual->filename, filename) == 0)
            wave = actual;

        last = actual;
        actual = actual->next;
    }

    /* If wav was not found */
    if (wave == NULL) {
        wave = calloc(1, sizeof *wave);
        wave->filename = filename;
        if (SDL_LoadWAV(filename, &wave->spec, &wave->buffer, &wave->length) == NULL) {
            free(wave);
            return;
        }

        last->next = wave;

        SDL_Log("new sound allocated");
    }

    wave->actual_buffer = wave->buffer;
    wave->actual_length = wave->length;
}

