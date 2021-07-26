#ifndef DRAW_H_
#define DRAW_H_

#include "tetromino.h"

#include <stdint.h>

#define SCREEN_X_OFFSET 1
#define SCREEN_Y_OFFSET 1

struct ConsoleSize
{
    int32_t width;
    int32_t height;
};

struct ConsoleSize get_console_size();
void screen_set_value(char *screen, char value, int32_t x, int32_t y, int32_t width);
char screen_get_value(char *screen, int32_t x, int32_t y, int32_t width);
void place_tetromino_to_screen(struct Tetromino *tetromino, char *screen, struct ConsoleSize *console_size);
void draw_tetromino(struct Tetromino *tetromino, vec2i coord, char *screen, struct ConsoleSize *console_size);
void place_landed_blocks_to_screen(int32_t *landed, char *screen, struct ConsoleSize *console_size);
void draw_frames(char *screen, int32_t width);
void draw_next_tetromino_preview(struct Tetromino *tetromino, char *screen, struct ConsoleSize *console_size);

#endif // DRAW_H_
