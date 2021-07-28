#ifndef DRAW_H_
#define DRAW_H_

#include "tetromino.h"

#include <stdint.h>

struct Buffer
{
    char *buffer;
    int32_t width;
    int32_t height;
    int32_t size_in_bytes;
};

void get_console_size(struct Buffer *console);

void buffer_set_value(struct Buffer *buffer, char value, int32_t x, int32_t y);
char buffer_get_value(struct Buffer *buffer, int32_t x, int32_t y);

void draw_tetromino(struct Buffer *console, struct Tetromino *tetromino, vec2i coord);
void draw_frame(struct Buffer *console, vec2i start_coord, vec2i end_coord);
void draw_next_tetromino_preview(struct Buffer *console, struct Tetromino *tetromino, vec2i coord);

void draw_field(struct Buffer* console, struct Buffer *field, vec2i coord);

void place_tetromino_to_field(struct Buffer *console, struct Tetromino *tetromino);
void place_landed_blocks_to_field(struct Buffer *console, int32_t *landed);

#endif // DRAW_H_
