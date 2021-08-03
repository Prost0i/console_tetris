#!/bin/sh

OPTS="-std=c99 -Wall -Wextra -Werror -pedantic -g"

clang main.c $OPTS -o build/tetris
