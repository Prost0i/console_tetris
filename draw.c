#include "tetromino.h"
#include "landed.h"

#include <Windows.h>

#include "draw.h"

// Getting console buffer width and height
struct ConsoleSize get_console_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    struct ConsoleSize console_size;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    console_size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return console_size;
}

void screen_set_value(char *screen, char value, int32_t x, int32_t y, int32_t width)
{
    screen[width*y+x] = value;
}

char screen_get_value(char *screen, int32_t x, int32_t y, int32_t width)
{
    return screen[width*y+x];
}

void place_tetromino_to_screen(struct Tetromino *tetromino, char *screen, struct ConsoleSize *console_size)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y) != 0)
            {
                const int32_t screen_x = x+tetromino->topLeft.x + SCREEN_X_OFFSET;
                const int32_t screen_y = y+tetromino->topLeft.y + SCREEN_Y_OFFSET;
                const int32_t screen_w = console_size->width;
                screen_set_value(screen, 'T', screen_x, screen_y, screen_w);
            }
        }
    }
}

void draw_tetromino(struct Tetromino *tetromino, vec2i coord, char *screen, struct ConsoleSize *console_size)
{
    for (int32_t y = 0; y < tetromino->shapes[0].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[0].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y) != 0)
            {
                const int32_t screen_x = x+coord.x;
                const int32_t screen_y = y+coord.y;
                const int32_t screen_w = console_size->width;
                screen_set_value(screen, 'T', screen_x, screen_y, screen_w);
            }
        }
    }
}

void place_landed_blocks_to_screen(int32_t *landed, char *screen, struct ConsoleSize *console_size)
{
    for (int32_t y = 0; y < LAND_HEIGHT; ++y)
    {
        for (int32_t x = 0; x < LAND_WIDTH; ++x)
        {
            const int32_t screen_x = x + SCREEN_X_OFFSET;
            const int32_t screen_y = y + SCREEN_Y_OFFSET;
            const int32_t screen_w = console_size->width;

            // Mapping landed tetris blocks to screen buffer
            if (landed_get_value(landed, x, y) != 0)
            {
                screen_set_value(screen, 'X', screen_x, screen_y, screen_w);
            }
            else
            {
                screen_set_value(screen, ' ', screen_x, screen_y, screen_w);
            }
        }
    }
}

void draw_frames(char *screen, int32_t width)
{
    // main play field frame:
    // drawing left and right boundary
    for (int i = 1; i < LAND_HEIGHT+1; ++i)
    {
        screen_set_value(screen, '|', 0, i, width);
        screen_set_value(screen, '|', 11, i, width);
    }
    // drawing angles
    screen_set_value(screen, '+', 0, 0, width);
    screen_set_value(screen, '+', 11, 0, width);
    screen_set_value(screen, '+', 0, 17, width);
    screen_set_value(screen, '+', 11, 17, width);
    // drawing ceiling and floor
    for (int i = 1; i < LAND_WIDTH+1; ++i)
    {
        screen_set_value(screen, '-', i, 0, width);
        screen_set_value(screen, '-', i, 17, width);
    }

    // next tetromino preview frame:
    // drawing right boundary
    for (int i = 1; i < 5; ++i)
    {
        screen_set_value(screen, '|', 16, i, width);
    }
    // drawing ceiling and floor
    for (int i = 12; i < 16; ++i)
    {
        screen_set_value(screen, '-', i, 0, width);
        screen_set_value(screen, '-', i, 5, width);
    }
    // drawing angles
    screen_set_value(screen, '+', 16, 0, width);
    screen_set_value(screen, '+', 16, 5, width);
}

void draw_next_tetromino_preview(struct Tetromino *tetromino, char *screen, struct ConsoleSize *console_size)
{
    // clearing preview window from previous tetromino
    for (int y = 1; y < 5; ++y)
    {
        for (int x = 12; x < 16; ++x)
        {
            screen_set_value(screen, ' ', x, y, console_size->width);
        }
    }

    // placing tetromino in preview window
    vec2i next_tetromino_coord = { .x = 12, .y = 1 };
    draw_tetromino(tetromino, next_tetromino_coord, screen, console_size);
}
