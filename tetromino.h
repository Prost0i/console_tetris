#ifndef TETROMINO_H_
#define TETROMINO_H_

#include "buffer.h"

#include <stdint.h>
#include <stdlib.h>

#define TETROMINO_SHAPE_SIZE 16
#define MAX_TETROMINO_SHAPE_STATES 4

typedef struct Vec2i
{
    int32_t x;
    int32_t y;
} vec2i;

// shape object
struct Shape
{
    int8_t shape[TETROMINO_SHAPE_SIZE];
    int32_t width;
    int32_t height;
};

// Tetromino coordinates and state of shape
struct Tetromino
{
    struct Shape shapes[MAX_TETROMINO_SHAPE_STATES];
    int32_t currentShape;
    int32_t countOfShapes;
    vec2i topLeft;
    vec2i potentialTopLeft;
};

void tetromino_set_value(struct Shape *tetrominoShape, int8_t value, int32_t x, int32_t y);
int8_t tetromino_get_value(struct Shape *tetrominoShape, int32_t x, int32_t y);
void reset_tetromino_state(struct Tetromino *tetromino);
void rotate_tetromino(struct Tetromino *tetromino, struct Buffer *landed);
void create_tetromino(struct Tetromino *tetromino, const char **values, const vec2i *sizes, size_t count);
void init_tetrominoes(struct Tetromino *tetrominoes);

#endif // TETROMINO_H_
