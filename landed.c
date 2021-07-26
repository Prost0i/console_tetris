#include "landed.h"

void landed_set_value(int32_t *landed, int32_t value, int32_t x, int32_t y)
{
    landed[LAND_WIDTH*y+x] = value;
}

int32_t landed_get_value(int32_t *landed, int32_t x, int32_t y)
{
    return landed[LAND_WIDTH*y+x];
}

void land_tetromino(struct Tetromino *tetromino, int32_t *landed)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            int32_t tetromino_value = tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y);
            if (tetromino_value != 0)
            {
                landed_set_value(landed, tetromino_value, x + tetromino->topLeft.x, y + tetromino->topLeft.y);
            }
        }
    }
}
