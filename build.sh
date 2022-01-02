#!/bin/sh

OPTS="-std=c99 -D_POSIX_C_SOURCE=199309L -Wall -Wextra -Werror -pedantic -g"
BUILD_DIR="./build"
EXE_PATH="$BUILD_DIR/tetris"
[ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
gcc main.c $OPTS -o $EXE_PATH
