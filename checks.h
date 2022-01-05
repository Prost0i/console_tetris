#ifndef CHECKS_H_

#include "tetromino.h"

#include <stdint.h>
#include <stdbool.h>

bool check_boundary(struct Tetromino *tetromino, struct Buffer *landed, int32_t x, int32_t y);
bool check_landing(struct Tetromino *tetromino, struct Buffer *landed, int32_t x, int32_t y);
bool check_colision(struct Tetromino *tetromino, struct Buffer *landed);
void check_filled_row(struct Buffer *landed, int32_t *score, double *falling_cooldown);
bool game_over_check(struct Tetromino *tetromino, struct Buffer *landed);

#endif // CHECKS_H_
