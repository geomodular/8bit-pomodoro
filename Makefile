
PLATS=linux osx em

all:
	@echo "make <linux|osx|em>"

$(PLATS):
	$(MAKE) -f Makefile.$@

clean:
	@rm -f pomodoro
	@rm -f pomodoro.html pomodoro.js pomodoro.html.mem pomodoro.data
