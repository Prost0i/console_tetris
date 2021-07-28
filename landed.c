#include "landed.h"
#include "draw.h"

void landed_set_value(struct Buffer *landed, int8_t value, int32_t x, int32_t y)
{
    landed->buffer[landed->width*y+x] = value;
}

int8_t landed_get_value(struct Buffer *landed, int32_t x, int32_t y)
{
    return landed->buffer[landed->width*y+x];
}

void land_tetromino(struct Tetromino *tetromino, struct Buffer *landed)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            int8_t tetromino_value = tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y);
            if (tetromino_value != 0)
            {
                landed_set_value(landed, tetromino_value, x + tetromino->topLeft.x, y + tetromino->topLeft.y);
            }
        }
    }
}
