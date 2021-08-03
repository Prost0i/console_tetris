#include "buffer.h"
#include "tetromino.h"
#include "draw.h"
#include "landed.h"
#include "checks.h"
#include "platform.h"

#include "buffer.c"
#include "tetromino.c"
#include "draw.c"
#include "landed.c"
#include "checks.c"

#ifdef _WIN32
#include "win32_platform.c"
#endif

#ifdef __unix__
#include "posix_platform.c"
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

struct Cooldowns {
    double x1, x2,
          y1, y2,
          r1, r2;
};

// Allocating screen buffer for rendering landed blocks and falling tetrominoes
char screen[1000*1000];

int main()
{
    srand((unsigned int)time(NULL));
    struct Buffer console = {
        .buffer = screen,
        .width = 0,
        .height = 0,
        .size_in_bytes = 0
    };
    init_console(&console);
    init_time();
    // Clear screen with space characters
    memset(console.buffer, ' ', console.size_in_bytes);

    // Play field frame drawing to console buffer
    const vec2i start_land_frame_coord = { .x = 0, .y = 0 };
    const vec2i end_land_frame_coord = { .x = 11, .y = 17 };
    draw_frame(&console, start_land_frame_coord, end_land_frame_coord);

    // Next tetromino preview frame drawing to console buffer
    const vec2i start_preview_frame_coord = { .x = 13, .y = 0 };
    const vec2i end_preview_frame_coord = { .x = 18, .y = 5 };
    draw_frame(&console, start_preview_frame_coord, end_preview_frame_coord);

    // State buffer of falling tetromino and landed blocks
    char field_buffer[10*16] = {0};
    struct Buffer field = {
        .buffer = field_buffer,
        .width = 10,
        .height = 16,
    };

    // landed blocks with field 10x16 (10 columns and 16 rows)
    char landed_buffer[10*16] = {0};
    struct Buffer landed = {
        .buffer = landed_buffer,
        .width = 10,
        .height = 16,
        .size_in_bytes = 10*16
    };

    struct Tetromino tetrominoes[7] = {0};
    init_tetrominoes(tetrominoes);

    struct Cooldowns cd;
    cd.x1 = get_time_in_seconds();
    cd.y1 = get_time_in_seconds();
    cd.r1 = get_time_in_seconds();

    // choose current tetromino
    int32_t next_tetromino = rand() % 7;
    // randomly choose the next value for next loop iteration
    int32_t current_tetromino = rand() % 7;

    int32_t score = 0;
    double falling_cooldown = 0.5;

    for (;;)
    {
        struct Keys keys;

        // input
        get_key(&keys);

        if (keys.escape)
        {
            break;
        }

        cd.y2 = get_time_in_seconds();
        // increased falling
        if (keys.down && time_diff(cd.y1, cd.y2) > (falling_cooldown / 5.0))
        {
            cd.y1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.y;
        }
        // normal falling
        else if (time_diff(cd.y1, cd.y2) > falling_cooldown)
        {
            cd.y1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.y;
        }

        cd.x2 = get_time_in_seconds();
        if (keys.left && time_diff(cd.x1, cd.x2) > 0.1)
        {
            cd.x1 = get_time_in_seconds();
            --tetrominoes[current_tetromino].potentialTopLeft.x;
        }
        else if (keys.right && time_diff(cd.x1, cd.x2) > 0.1)
        {
            cd.x1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.x;
        }
        
        cd.r2 = get_time_in_seconds();
        if (keys.up && time_diff(cd.r1, cd.r2) > 0.2)
        {
            cd.r1 = get_time_in_seconds();
            rotate_tetromino(&tetrominoes[current_tetromino], &landed);
        }

        if (check_colision(&tetrominoes[current_tetromino], &landed))
        {
            // if row filled increasing score and falling down speed 
            check_filled_row(&landed, &score, &falling_cooldown);
            if (game_over_check(&tetrominoes[next_tetromino], &landed))
            {
                break;
            }

            current_tetromino = next_tetromino;
            next_tetromino = rand() % 7;
        }
        else
        {
            tetrominoes[current_tetromino].topLeft = tetrominoes[current_tetromino].potentialTopLeft;
        }

        place_landed_blocks_to_field(&field, &landed);
        place_tetromino_to_field(&field, &tetrominoes[current_tetromino]);

        const vec2i tetromino_preview_coord = {
            .x = start_preview_frame_coord.x+1,
            .y = start_preview_frame_coord.y+1
        };
        draw_next_tetromino_preview(&console, &tetrominoes[next_tetromino], tetromino_preview_coord);

        const vec2i field_coord = { .x = 1, .y = 1 };
        draw_field(&console, &field, field_coord);

        // Copy screen buffer to console buffer
        write_to_console(&console);
    }
    // Copy last frame to console buffer
    write_to_console(&console);

    close_console(&console);

    printf("Score: %d\n", score);
    return 0;
}
