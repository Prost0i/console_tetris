#ifndef CHECKS_H_

#include "tetromino.h"

#include <stdint.h>

int32_t check_boundary(struct Tetromino *tetromino, int32_t *landed, int32_t x, int32_t y);
int32_t check_landing(struct Tetromino *tetromino, int32_t *landed, int32_t x, int32_t y);
int32_t check_colision(struct Tetromino *tetromino, int32_t *landed);
void check_filled_row(int32_t *landed, int32_t *score, float *falling_cooldown);
int32_t game_over_check(struct Tetromino *tetromino, int32_t *landed);

#endif // CHECKS_H_
