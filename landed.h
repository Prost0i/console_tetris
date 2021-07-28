#ifndef LANDED_H_

#include "tetromino.h"

#include <stdint.h>

void landed_set_value(struct Buffer *landed, int8_t value, int32_t x, int32_t y);
int8_t landed_get_value(struct Buffer *landed, int32_t x, int32_t y);
void land_tetromino(struct Tetromino *tetromino, struct Buffer *landed);

#endif // LANDED_H_
