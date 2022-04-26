#!/bin/bash
gcc-11 -o yance.app yance.c -I/opt/homebrew/include -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf \
&& ./yance.app
