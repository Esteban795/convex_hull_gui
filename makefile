SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDANCES = $(SRCDIR)/utilities/custom_stack.c $(SRCDIR)/utilities/array_manipulation.c $(SRCDIR)/SDL_utilities/drawers.c $(SRCDIR)/utilities/geometry.c

build:
	gcc $(SRCDIR)/convex_hull_gui.c -o $(LIBDIR)/convex_hull_gui $(CFLAGS) $(DEPENDANCES)

run: $(SRCDIR)/convex_hull_gui
	$(SRCDIR)/convex_hull_gui $(SRCDIR)/test.txt

randomize_points: file_generator.c 
	gcc $(SRCDIR)/utilities/file_generator.c -o $(SRCDIR)/utilities/fg

clean:
	rm $(SRCDIR)/convex_hull_gui

all:
	make build
	make run