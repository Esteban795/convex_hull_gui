SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDANCES = $(SRCDIR)/utilities/custom_stack.c $(SRCDIR)/utilities/array_manipulation.c $(SRCDIR)/SDL_utilities/drawers.c $(SRCDIR)/utilities/geometry.c

build:
	gcc $(SRCDIR)/convex_hull_gui.c -o bin/convex_hull_gui $(CFLAGS) $(DEPENDANCES)

run : bin/convex_hull_gui
	bin/convex_hull_gui $(PATH);clear

clean:
	rm bin/convex_hull_gui
