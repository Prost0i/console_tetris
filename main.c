#include "tetromino.h"
#include "draw.h"
#include "landed.h"
#include "checks.h"

#include "tetromino.c"
#include "draw.c"
#include "landed.c"
#include "checks.c"

#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

struct Keys
{
    bool left;
    bool right;
    bool up;
    bool down;
    bool escape;
};

float time_diff(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER cpu_freq)
{
    float result = ((float)(end.QuadPart - start.QuadPart) / (float)cpu_freq.QuadPart);
    return result;
}

// Allocating screen buffer for rendering landed blocks and falling tetrominoes
char screen[1000*1000];

int main()
{
    LARGE_INTEGER cpu_freq;
    QueryPerformanceFrequency(&cpu_freq);
    srand((unsigned int)time(NULL));
    struct Buffer console = {
        .buffer = screen,
        .width = 0,
        .height = 0,
        .size_in_bytes = 0
    };
    get_console_size(&console);
    // Console Raw output initiliazation
    HANDLE console_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console_handle);
    DWORD bytes_written = 0;
    const COORD zero_coord = {
        .X = 0,
        .Y = 0,
    };
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

    // landed blocks with field 10x16 (10 columns and 16 rows)
    int32_t landed[LAND_SIZE] = {0};

    // State buffer of falling tetromino and landed blocks
    char field_buffer[10*16];
    struct Buffer field = {
        .buffer = field_buffer,
        .width = 10,
        .height = 16
    };

    struct Tetromino tetrominoes[7] = {0};
    init_tetrominoes(tetrominoes);

    // TODO Remove all this QPC win32 bullshit!!
    LARGE_INTEGER x_cooldown1, x_cooldown2,
                  y_cooldown1, y_cooldown2,
                  r_cooldown1, r_cooldown2;

    QueryPerformanceCounter(&x_cooldown1);
    QueryPerformanceCounter(&y_cooldown1);
    QueryPerformanceCounter(&r_cooldown1);
    int32_t score = 0;
    // choose current tetromino
    int32_t next_tetromino = rand() % 7;
    float falling_cooldown = 0.5f;

    for (;;)
    {
        struct Keys keys;
        // setting current tetromino by next_tetromino value from previous loop iteration
        int32_t current_tetromino = next_tetromino;
        // randomly choose the next value for next loop iteration
        next_tetromino = rand() % 7;

        if (game_over_check(&tetrominoes[current_tetromino], landed))
        {
            goto exit;
        }

        const vec2i tetromino_preview_coord = {
            .x = start_preview_frame_coord.x+1,
            .y = start_preview_frame_coord.y+1
        };
        draw_next_tetromino_preview(&console, &tetrominoes[next_tetromino], tetromino_preview_coord);

        for (;;)
        {
            // input
            keys.left = GetAsyncKeyState(VK_LEFT);
            keys.right = GetAsyncKeyState(VK_RIGHT);
            keys.up = GetAsyncKeyState(VK_UP);
            keys.down = GetAsyncKeyState(VK_DOWN);
            keys.escape = GetAsyncKeyState(VK_ESCAPE);

            if (keys.escape)
            {
                goto exit;
            }

            const vec2i field_coord = { .x = 1, .y = 1 };
            place_landed_blocks_to_field(&field, landed);

            QueryPerformanceCounter(&y_cooldown2);
            // increased falling
            if (keys.down && time_diff(y_cooldown1, y_cooldown2, cpu_freq) > (falling_cooldown / 5.0f))
            {
                QueryPerformanceCounter(&y_cooldown1);
                ++tetrominoes[current_tetromino].potentialTopLeft.y;
            }
            // normal falling
            else if (time_diff(y_cooldown1, y_cooldown2, cpu_freq) > falling_cooldown)
            {
                QueryPerformanceCounter(&y_cooldown1);
                ++tetrominoes[current_tetromino].potentialTopLeft.y;
            }

            QueryPerformanceCounter(&x_cooldown2);
            if (keys.left && time_diff(x_cooldown1, x_cooldown2, cpu_freq) > 0.1f)
            {
                QueryPerformanceCounter(&x_cooldown1);
                --tetrominoes[current_tetromino].potentialTopLeft.x;
            }
            else if (keys.right && time_diff(x_cooldown1, x_cooldown2, cpu_freq) > 0.1f)
            {
                QueryPerformanceCounter(&x_cooldown1);
                ++tetrominoes[current_tetromino].potentialTopLeft.x;
            }
            
            QueryPerformanceCounter(&r_cooldown2);
            if (keys.up && time_diff(r_cooldown1, r_cooldown2, cpu_freq) > 0.2f)
            {
                QueryPerformanceCounter(&r_cooldown1);
                rotate_tetromino(&tetrominoes[current_tetromino], landed);
            }

            place_tetromino_to_field(&field, &tetrominoes[current_tetromino]);

            if (check_colision(&tetrominoes[current_tetromino], landed))
            {
                // if row filled increasing score and falling down speed 
                check_filled_row(landed, &score, &falling_cooldown);
                break;
            }
            else
            {
                tetrominoes[current_tetromino].topLeft = tetrominoes[current_tetromino].potentialTopLeft;
            }

            // Copy field buffer to console buffer
            draw_field(&console, &field, field_coord);

            // Copy screen buffer to console buffer
            WriteConsoleOutputCharacter(console_handle, console.buffer, console.width * console.height, zero_coord, &bytes_written);
        }
    }

exit:
    // Copy last frame to console buffer
    WriteConsoleOutputCharacter(console_handle, console.buffer, console.width * console.height, zero_coord, &bytes_written);
    memset(console.buffer, ' ', console.size_in_bytes);
    sprintf(console.buffer, "Score: %d", score);
    screen[strlen(screen)] = ' ';
    WriteConsoleOutputCharacter(console_handle, console.buffer, console.width * console.height, zero_coord, &bytes_written);
    Sleep(2000);

    printf("Score: %d\n", score);
    return 0;
}
