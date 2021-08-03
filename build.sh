#!/bin/sh

OPTS="-std=c99 -Wall -Wextra -Werror -pedantic -g"
BUILD_DIR="./build"
EXE_PATH="$BUILD_DIR/tetris"
[ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
clang main.c $OPTS -o $EXE_PATH
