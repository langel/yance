#!/bin/bash

#echo gcc-11 -o main.app main.c $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf
#gcc-11 -o main.app main.c $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf
gcc-11 -o main.app main.c -I/opt/homebrew/include -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf \
&& ./main.app
