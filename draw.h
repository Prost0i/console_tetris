#ifndef DRAW_H_
#define DRAW_H_

#include "buffer.h"
#include "tetromino.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void draw_frame(struct Buffer *console, vec2i start_coord, vec2i end_coord);
void draw_next_tetromino_preview(struct Buffer *console, struct Tetromino *tetromino, vec2i coord);

void draw_field(struct Buffer* console, struct Buffer *field, vec2i coord);

void place_tetromino_to_field(struct Buffer *console, struct Tetromino *tetromino);
void place_landed_blocks_to_field(struct Buffer *console, struct Buffer *landed);

#endif // DRAW_H_
