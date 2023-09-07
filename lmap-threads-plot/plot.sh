#!/bin/bash
gcc lmap-threads.c -o lmap-threads.exe -lm -pthread
rm valores.dat
touch valores.dat

gnuplot plot.gnu &
./lmap-threads.exe
