#!/bin/bash
gcc lmap_pipe.c -o lmap_pipe.exe -lm
rm valores.dat
touch valores.dat

gnuplot plot.gnu &
./lmap_pipe.exe
