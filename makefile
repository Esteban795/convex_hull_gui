SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -std=c99 -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDANCES = $(SRCDIR)/utilities/custom_stack.c $(SRCDIR)/utilities/array_manipulation.c $(SRCDIR)/SDL_utilities/drawers.c $(SRCDIR)/utilities/geometry.c

build:
	gcc $(SRCDIR)/convex_hull_gui.c -o $(LIBDIR)/convex_hull_gui.out $(CFLAGS) $(DEPENDANCES)

run: convex_hull_gui.out
	./convex_hull_gui.out

all:
	build run