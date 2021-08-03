#!/bin/sh

OPTS="-std=c99 -Wall -Wextra -Werror -pedantic -O3"
BUILD_DIR="./build_rel"
EXE_PATH="$BUILD_DIR/tetris"
[ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
clang main.c $OPTS -o $EXE_PATH && strip $EXE_PATH
