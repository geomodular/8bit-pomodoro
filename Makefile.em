
pomodoro.html: main.c db16.h db16.c screen.h screen.c display.h display.c buttons.h buttons.c widget.h widget.c wave.h wave.c
	emcc -O2 -s USE_SDL=2 -Wall -pedantic -std=c11 main.c db16.c screen.c display.c buttons.c widget.c wave.c --preload-file gfx.bmp --preload-file start.wav --preload-file click.wav -o pomodoro.html

