#ifndef LANDED_H_

#include "tetromino.h"

#include <stdint.h>

#define LAND_WIDTH 10
#define LAND_HEIGHT 16
#define LAND_SIZE LAND_WIDTH*LAND_HEIGHT

void landed_set_value(int32_t *landed, int32_t value, int32_t x, int32_t y);
int32_t landed_get_value(int32_t *landed, int32_t x, int32_t y);
void land_tetromino(struct Tetromino *tetromino, int32_t *landed);

#endif // LANDED_H_
