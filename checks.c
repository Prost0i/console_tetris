#include "landed.h"

#include <stdbool.h>

#include "checks.h"

bool check_boundary(struct Tetromino *tetromino, struct Buffer *landed, int32_t x, int32_t y)
{
    // check for left boundary
    if (x + tetromino->potentialTopLeft.x < 0)
    {
        return true;
    }
    // check for right boundary
    if (x + tetromino->potentialTopLeft.x >= landed->width)
    {
        return true;
    }
    // check left and right landed blocks
    if (landed_get_value(landed,
                         x + tetromino->potentialTopLeft.x,
                         y + tetromino->topLeft.y) != 0)
    {
        return true;
    }
    return false;
}

// returning 0 if tetromino is not landed
// returning 1 if tetromino is landed
bool check_landing(struct Tetromino *tetromino, struct Buffer *landed, int32_t x, int32_t y)
{
    // landing on floor
    if (y + tetromino->potentialTopLeft.y >= landed->height)
    {
        return true;
    }
    // landing on landed blocks
    if (landed_get_value(landed,
                         x + tetromino->potentialTopLeft.x,
                         y + tetromino->potentialTopLeft.y) != 0)
    {
        return true;
    }
    return false;
}

// returning 0 if tetromino is not landed
// returning 1 if tetromino is landed
bool check_colision(struct Tetromino *tetromino, struct Buffer *landed)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y) != 0)
            {
                if (check_boundary(tetromino, landed, x, y))
                {
                    tetromino->potentialTopLeft.x = tetromino->topLeft.x;
                }
                if (check_landing(tetromino, landed, x, y))
                {
                    land_tetromino(tetromino, landed);
                    reset_tetromino_state(tetromino);
                    return true;
                }
            }
        }
    }
    return false;
}

void check_filled_row(struct Buffer *landed, int32_t *score, double *falling_cooldown)
{
    for (int32_t y = 0; y < landed->height; ++y)
    {
        bool is_filled = true;
        for (int32_t x = 0; x < landed->width; ++x)
        {
            if (landed_get_value(landed, x, y) == 0)
            {
                is_filled = false;
            }
        }
        if (is_filled)
        {
            // increasing game score
            *score += 100;
            // increasing falling speed
            *falling_cooldown *= 0.95f;
            for (int32_t x = 0; x < landed->width; ++x)
            {
                landed_set_value(landed, 0, x, y);
            }
            
            for (int32_t yy = y-1; yy > 0; --yy)
            {
                for (int32_t x = 0; x < landed->width; ++x)
                {
                    int8_t value = landed_get_value(landed, x, yy);
                    landed_set_value(landed, value, x, yy+1);
                }
            }
            for (int32_t x = 0; x < landed->width; ++x)
            {
                landed_set_value(landed, 0, x, 0);
            }
        }
    }
}

bool game_over_check(struct Tetromino *tetromino, struct Buffer *landed)
{
    // if tetromino spawned in landed blocks game is over
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            if (landed_get_value(landed,
                                 x + tetromino->topLeft.x,
                                 y + tetromino->topLeft.y) != 0)
            {
                return true;
            }
        }
    }
    return false;
}
