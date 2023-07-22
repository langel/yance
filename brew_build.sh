#!/bin/bash
gcc -o yance.app yance.c $(sdl2-config --cflags --static-libs) 
./yance.app
