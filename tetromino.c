#include "landed.h"
#include "checks.h"

#include "tetromino.h"

void tetromino_set_value(struct Shape *tetrominoShape, int32_t value, int32_t x, int32_t y)
{
    tetrominoShape->shape[tetrominoShape->width*y+x] = value;
}

int32_t tetromino_get_value(struct Shape *tetrominoShape, int32_t x, int32_t y)
{
    return tetrominoShape->shape[tetrominoShape->width*y+x];
}

void reset_tetromino_state(struct Tetromino *tetromino)
{
    tetromino->currentShape = 0;
    tetromino->topLeft.x = 4;
    tetromino->topLeft.y = 0;
    tetromino->potentialTopLeft = tetromino->topLeft;
}

void rotate_tetromino(struct Tetromino *tetromino, int32_t *landed)
{
    if (tetromino->countOfShapes <= 1)
    {
        return;
    }

    int32_t next_shape_index = 
        ((tetromino->currentShape+1) >= tetromino->countOfShapes)
        ? 0
        : tetromino->currentShape+1;

    struct Shape *shape = &tetromino->shapes[next_shape_index];

    // check collision for next shape
    for (int32_t y = 0; y < shape->height; ++y)
    {
        for (int32_t x = 0; x < shape->width; ++x)
        {
            if (tetromino_get_value(shape, x, y) != 0)
            {
                if (check_boundary(tetromino, landed, x, y))
                {
                    return;
                }

                if (check_landing(tetromino, landed, x, y))
                {
                    return;
                }
            }
        }
    }

    tetromino->currentShape = next_shape_index;
}

void create_tetromino(struct Tetromino *tetromino, const char **values, const vec2i *sizes, size_t count)
{
    tetromino->countOfShapes = count;
    reset_tetromino_state(tetromino);
    for (size_t i = 0; i < count; ++i)
    {
        struct Shape shape;
        shape.width = sizes[i].x;
        shape.height = sizes[i].y;
        for (int32_t y = 0; y < shape.height; ++y)
        {
            for (int32_t x = 0; x < shape.width; ++x)
            {
                if (values[i][shape.width*y+x] == 'X')
                {
                    tetromino_set_value(&shape, 1, x, y);
                }
                else if (values[i][shape.width*y+x] == '0')
                {
                    tetromino_set_value(&shape, 0, x, y);
                }
            }
        }

        tetromino->shapes[i] = shape;
    }
}

void init_tetrominoes(struct Tetromino *tetrominoes)
{
    // I
    const char *IVal[2] = {
        "XXXX",

        "X"
        "X"
        "X"
        "X"
    };
    const vec2i Isizes[2] =
    {
        { .x = 4, .y = 1},
        { .x = 1, .y = 4},
    };
    create_tetromino(&tetrominoes[0], IVal, Isizes, 2);

    // O
    const char *OVal[1] = {
        "XX"
        "XX",
    };
    const vec2i Osizes[1] =
    {
        { .x = 2, .y = 2 }
    };
    create_tetromino(&tetrominoes[1], OVal, Osizes, 1);

    // T
    const char *TVal[4] = {
        "XXX"
        "0X0",

        "0X"
        "XX"
        "0X",

        "0X0"
        "XXX",

        "X0"
        "XX"
        "X0",
    };
    const vec2i Tsizes[4] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3}
    };
    create_tetromino(&tetrominoes[2], TVal, Tsizes, 4);

    // J
    const char *JVal[4] = {
        "0X"
        "0X"
        "XX",

        "X00"
        "XXX",

        "XX"
        "X0"
        "X0",

        "XXX"
        "00X"
    };
    const vec2i Jsizes[4] =
    {
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2}
    };
    create_tetromino(&tetrominoes[3], JVal, Jsizes, 4);

    // L
    const char *LVal[4] = {
        "XX"
        "0X"
        "0X",

        "00X"
        "XXX",

        "X0"
        "X0"
        "XX",

        "XXX"
        "X00"
    };
    const vec2i Lsizes[4] =
    {
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2}
    };
    create_tetromino(&tetrominoes[4], LVal, Lsizes, 4);

    // S
    const char *SVal[2] = {
        "0XX"
        "XX0",

        "X0"
        "XX"
        "0X"
    };
    const vec2i Ssizes[2] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
    };
    create_tetromino(&tetrominoes[5], SVal, Ssizes, 2);

    // Z
    const char *ZVal[2] = {
        "XX0"
        "0XX",

        "0X"
        "XX"
        "X0"
    };
    const vec2i Zsizes[2] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
    };
    create_tetromino(&tetrominoes[6], ZVal, Zsizes, 2);
}
