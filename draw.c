#include "tetromino.h"
#include "landed.h"

#include "draw.h"

void clear_screen_with(struct Buffer *console, char ch)
{
    memset(console->buffer, ch, console->size_in_bytes);
}

void draw_text(struct Buffer *console, vec2i pos, const char *text, size_t length)
{
    char *where = console->buffer+(console->width*pos.y+pos.x);
    for (size_t i = 0; i < length-1; ++i)
    {
	    where[i] = text[i];
    }
}

void draw_score(struct Buffer *console, vec2i pos, int32_t score)
{
    char buf[255];
    size_t length = sprintf(buf, "Score: %d", score);
    length += 1;
    draw_text(console, pos, buf, length);
}

void draw_frame(struct Buffer *console, vec2i start_coord, vec2i end_coord)
{
    // drawing angles
    buffer_set_value(console, '+', start_coord.x, start_coord.y);
    buffer_set_value(console, '+', end_coord.x, start_coord.y);
    buffer_set_value(console, '+', start_coord.x, end_coord.y);
    buffer_set_value(console, '+', end_coord.x, end_coord.y);
    
    // drawing left and right boundary
    for (int32_t i = start_coord.y+1; i < end_coord.y; ++i)
    {
        buffer_set_value(console, '|', start_coord.x, i);
        buffer_set_value(console, '|', end_coord.x, i);
    }
    // drawing ceiling and floor
    for (int32_t i = start_coord.x+1; i < end_coord.x; ++i)
    {
        buffer_set_value(console, '-', i, start_coord.y);
        buffer_set_value(console, '-', i, end_coord.y);
    }
}

#define PREVIEW_HEIGHT 4
#define PREVIEW_WIDTH 4

void draw_next_tetromino_preview(struct Buffer *console, struct Tetromino *tetromino, vec2i coord)
{
    const int32_t max_y = coord.y+PREVIEW_HEIGHT;
    const int32_t max_x = coord.x+PREVIEW_WIDTH;
    // clearing preview window from previous tetromino
    for (int32_t y = coord.y; y < max_y; ++y)
    {
        for (int32_t x = coord.x; x < max_x; ++x)
        {
            buffer_set_value(console, ' ', x, y);
        }
    }
    
    // placing tetromino in preview window
    for (int32_t y = 0; y < tetromino->shapes[0].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[0].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[0], x, y) != 0)
            {
                const int32_t screen_x = x+coord.x;
                const int32_t screen_y = y+coord.y;
                buffer_set_value(console, 'T', screen_x, screen_y);
            }
        }
    }
}

void draw_field(struct Buffer* console, struct Buffer *field, vec2i coord)
{
    for (int32_t y = 0; y < field->height; ++y)
    {
        for (int32_t x = 0; x < field->width; ++x)
        {
            int32_t screen_x = x + coord.x;
            int32_t screen_y = y + coord.y;
            char value = buffer_get_value(field, x, y);
            buffer_set_value(console, value, screen_x, screen_y);
        }
    }
}

void place_tetromino_to_field(struct Buffer *field, struct Tetromino *tetromino)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y) != 0)
            {
                const int32_t screen_x = x+tetromino->topLeft.x;
                const int32_t screen_y = y+tetromino->topLeft.y;
                buffer_set_value(field, 'T', screen_x, screen_y);
            }
        }
    }
}

void place_landed_blocks_to_field(struct Buffer *field, struct Buffer *landed)
{
    for (int32_t y = 0; y < landed->height; ++y)
    {
        for (int32_t x = 0; x < landed->width; ++x)
        {
            int32_t screen_x = x;
            int32_t screen_y = y;
            // Mapping landed tetris blocks to field buffer
            if (landed_get_value(landed, x, y) != 0)
            {
                buffer_set_value(field, 'X', screen_x, screen_y);
            }
            else
            {
                buffer_set_value(field, ' ', screen_x, screen_y);
            }
        }
    }
}
