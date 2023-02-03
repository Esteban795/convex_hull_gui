# Convex hull GUI using Graham's scan algorithm made in C.

Hey, welcome to this page.

## Motivation 

I recently felt very interested (for some reasons, don't ask me why, I don't know either) in computational geometry. So I decided to learn some basics algorithm, and I heard of Convex hull problem (I knew already what it was thanks to math classes) and well, I wanted to make my own, and use C at the same time, a language I'm currently learning as part of studies.
So I started.


## What does it do ?

Well, the name of the repo explains it quite clearly. You give the algorithm a set of points ((x,y) coordinates, Graham's scan doesn't work in 3D) and it finds you the convex hull of this set, and displays both the process and the result.

There is still something I need to work on : while the animation renders, framerate is quite low due to the use of SDL_Delay() to display, and let people watch how it works. I think I could use multi-threading to both draw and let people zoom/dezoom or move around at the same time, but this is currently not done.

## What did I learn ?

Pretty much everything : 
- I didn't know anything about computational geometry, neither SDL2 (which I've used in this project). 
- I managed to setup a 2D camera, a zoom system, a proper animation system.
- Basic geometry with computers.
- Project management in C (didn't really know what kind of architecture to follow). 
- Makefile : this was the first time I really used them, so I learned quite a lot and how practical they can be. Will definitely include them in every next project !


## **Requirements**

- SDL2 is quite obviously a must, since the rendering lib used is SDL2. You can download it on [SDL](https://www.libsdl.org/) (make sure you linked the .dll file correctly, of course) if you're on Windows, or just do 
```ps 
sudo apt get libsdl2-dev
```
if you're on Linux.

On MacOS, I have no clue if it works since I couldn't test it.

- GCC. or any C compiler should do the job. I've tested with GCC and Clang, and it seems to work fine for me. Don't be afraid to open an issue if it doesn't.


## How to install it on your computer ?

- Just clone it wherever you want in your computer.

```ps
git clone https://github.com/Esteban795/convex_hull_gui.git
```

- After that, head to the directory you cloned it, and type 
```ps
make all
``` 
It should run just fine with an example I made in the `tests` folder.

- If you want to run the program with one of your own file, make sure it follows the following pattern : 
```
n (number of points)
x1 y1
x2 y2
.
.
.
xn yn
```
Then, use 
```ps
make build
```
and finally,
```ps
make run PATH="YourFilePathHere"
```


- In case you don't want to keep the generated binary file the binary, please run
```ps
make clean
```
after you're done with it, to remove the binary file from your system.

## What's left to do ?

- A proper animation-not-tied-to-framerate system (I'll need to figure this one out later..)


### Hope you liked it !
