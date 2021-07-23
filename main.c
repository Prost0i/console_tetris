#include <Windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef NDEBUG
#define STB_LEAKCHECK_IMPLEMENTATION
#include <stb_leakcheck.h>
#endif // !NDEBUG

#define LAND_WIDTH 10
#define LAND_HEIGHT 16
#define LAND_SIZE LAND_WIDTH*LAND_HEIGHT
#define TETROMINO_SHAPE_SIZE 16
#define SCREEN_X_OFFSET 0
#define SCREEN_Y_OFFSET 0

struct ConsoleSize
{
    int32_t width;
    int32_t height;
};

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

typedef struct Vec2i
{
    int32_t x;
    int32_t y;
} vec2i;

// shape object
struct Shape
{
    int32_t shape[16];
    int32_t width;
    int32_t height;
};

// Tetromino coordinates and state of shape
struct Tetromino
{
    struct Shape shapes[4];
    int32_t currentShape;
    int32_t countOfShapes;
    vec2i topLeft;
    vec2i potentialTopLeft;
} Tetromino;

// Offsets for 2D arrays
inline void screen_set_value(char *screen, char value, int32_t x, int32_t y, int32_t width)
{
    screen[width*y+x] = value;
}

inline char screen_get_value(char *screen, int32_t x, int32_t y, int32_t width)
{
    return screen[width*y+x];
}

inline void landed_set_value(int32_t *landed, int32_t value, int32_t x, int32_t y)
{
    landed[10*y+x] = value;
}

inline int32_t landed_get_value(int32_t *landed, int32_t x, int32_t y)
{
    return landed[10*y+x];
}

inline void tetromino_set_value(struct Shape *tetrominoShape, int32_t value, int32_t x, int32_t y)
{
    tetrominoShape->shape[tetrominoShape->width*y+x] = value;
}

inline int32_t tetromino_get_value(struct Shape *tetrominoShape, int32_t x, int32_t y)
{
    return tetrominoShape->shape[tetrominoShape->width*y+x];
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

void place_landed_blocks_to_screen(int32_t *landed, char *screen, struct ConsoleSize *console_size)
{
    for (int32_t y = 0; y < 16; ++y)
    {
        for (int32_t x = 0; x < 10; ++x)
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

void land_tetromino(struct Tetromino *tetromino, int32_t *landed)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            int32_t tetromino_value = tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y);
            if (tetromino_value != 0)
            {
                landed_set_value(landed, tetromino_value, x + tetromino->topLeft.x, y + tetromino->topLeft.y);
            }
        }
    }
}

void reset_tetromino_state(struct Tetromino *tetromino)
{
    tetromino->currentShape = 0;
    tetromino->topLeft.x = 4;
    tetromino->topLeft.y = 0;
    tetromino->potentialTopLeft = tetromino->topLeft;
}

int32_t check_boundary(struct Tetromino *tetromino, int32_t *landed, int32_t x, int32_t y)
{
    // check for left boundary
    if (x + tetromino->potentialTopLeft.x < 0)
    {
        return 1;
    }
    // check for right boundary
    if (x + tetromino->potentialTopLeft.x >= LAND_WIDTH)
    {
        return 1;
    }
    // check left and right landed blocks
    if (landed_get_value(landed,
                x + tetromino->potentialTopLeft.x,
                y + tetromino->topLeft.y) != 0)
    {
        return 1;
    }
    return 0;
}

// returning 0 if tetromino is not landed
// returning 1 if tetromino is landed
int32_t check_landing(struct Tetromino *tetromino, int32_t *landed, int32_t x, int32_t y)
{
    // landing on floor
    if (y + tetromino->potentialTopLeft.y >= LAND_HEIGHT)
    {
        return 1;
    }
    // landing on landed blocks
    if (landed_get_value(landed,
                x + tetromino->potentialTopLeft.x,
                y + tetromino->potentialTopLeft.y) != 0)
    {
        return 1;
    }
    return 0;
}

// returning 0 if tetromino is not landed
// returning 1 if tetromino is landed
int32_t check_colision(struct Tetromino *tetromino, int32_t *landed)
{
    for (int32_t y = 0; y < tetromino->shapes[tetromino->currentShape].height; ++y)
    {
        for (int32_t x = 0; x < tetromino->shapes[tetromino->currentShape].width; ++x)
        {
            if (tetromino_get_value(&tetromino->shapes[tetromino->currentShape], x, y) != 0)
            {
                if (check_boundary(tetromino, landed, x, y))
                {
                    tetromino->potentialTopLeft.x = tetromino->topLeft.x;
                }
                if (check_landing(tetromino, landed, x, y))
                {
                    land_tetromino(tetromino, landed);
                    reset_tetromino_state(tetromino);
                    return 1;
                }
            }
        }
    }
    return 0;
}
void rotate_tetromino(struct Tetromino *tetromino, int32_t *landed)
{
    if (tetromino->countOfShapes <= 1)
    {
        return;
    }

    int32_t next_shape_index = 
        ((tetromino->currentShape+1) >= tetromino->countOfShapes)
        ? 0
        : tetromino->currentShape+1;

    struct Shape *shape = &tetromino->shapes[next_shape_index];

    // check collision for next shape
    for (int32_t y = 0; y < shape->height; ++y)
    {
        for (int32_t x = 0; x < shape->width; ++x)
        {
            if (tetromino_get_value(shape, x, y) != 0)
            {
                if (check_boundary(tetromino, landed, x, y))
                {
                    return;
                }

                if (check_landing(tetromino, landed, x, y))
                {
                    return;
                }
            }
        }
    }

    tetromino->currentShape = next_shape_index;
}

inline float time_diff(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER cpu_freq)
{
    float result = ((float)(end.QuadPart - start.QuadPart) / (float)cpu_freq.QuadPart);
    return result;
}

void create_tetromino(struct Tetromino *tetromino, const char **values, const vec2i *sizes, size_t count)
{
    tetromino->countOfShapes = count;
    reset_tetromino_state(tetromino);
    for (size_t i = 0; i < count; ++i)
    {
        struct Shape shape;
        shape.width = sizes[i].x;
        shape.height = sizes[i].y;
        for (int32_t y = 0; y < shape.height; ++y)
        {
            for (int32_t x = 0; x < shape.width; ++x)
            {
                if (values[i][shape.width*y+x] == 'X')
                {
                    tetromino_set_value(&shape, 1, x, y);
                }
                else if (values[i][shape.width*y+x] == '0')
                {
                    tetromino_set_value(&shape, 0, x, y);
                }
            }
        }

        tetromino->shapes[i] = shape;
    }
}

void init_tetrominoes(struct Tetromino *tetrominoes)
{
    // I
    const char *IVal[2] = {
        "XXXX",

        "X"
        "X"
        "X"
        "X"
    };
    const vec2i Isizes[2] =
    {
        { .x = 4, .y = 1},
        { .x = 1, .y = 4},
    };
    create_tetromino(&tetrominoes[0], IVal, Isizes, 2);

    // O
    const char *OVal[1] = {
        "XX"
        "XX",
    };
    const vec2i Osizes[1] =
    {
        { .x = 2, .y = 2 }
    };
    create_tetromino(&tetrominoes[1], OVal, Osizes, 1);

    // T
    const char *TVal[4] = {
        "XXX"
        "0X0",

        "0X"
        "XX"
        "0X",

        "0X0"
        "XXX",

        "X0"
        "XX"
        "X0",
    };
    const vec2i Tsizes[4] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3}
    };
    create_tetromino(&tetrominoes[2], TVal, Tsizes, 4);

    // J
    const char *JVal[4] = {
        "0X"
        "0X"
        "XX",

        "X00"
        "XXX",

        "XX"
        "X0"
        "X0",

        "XXX"
        "00X"
    };
    const vec2i Jsizes[4] =
    {
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2}
    };
    create_tetromino(&tetrominoes[3], JVal, Jsizes, 4);

    // L
    const char *LVal[4] = {
        "XX"
        "0X"
        "0X",

        "00X"
        "XXX",

        "X0"
        "X0"
        "XX",

        "XXX"
        "X00"
    };
    const vec2i Lsizes[4] =
    {
        { .x = 2, .y = 3},
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
        { .x = 3, .y = 2}
    };
    create_tetromino(&tetrominoes[4], LVal, Lsizes, 4);

    // S
    const char *SVal[2] = {
        "0XX"
        "XX0",

        "X0"
        "XX"
        "0X"
    };
    const vec2i Ssizes[2] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
    };
    create_tetromino(&tetrominoes[5], SVal, Ssizes, 2);

    // Z
    const char *ZVal[2] = {
        "XX0"
        "0XX",

        "0X"
        "XX"
        "X0"
    };
    const vec2i Zsizes[2] =
    {
        { .x = 3, .y = 2},
        { .x = 2, .y = 3},
    };
    create_tetromino(&tetrominoes[6], ZVal, Zsizes, 2);
}


int main()
{
    LARGE_INTEGER cpu_freq;
    QueryPerformanceFrequency(&cpu_freq);
    srand(time(NULL));
    struct ConsoleSize console_size = get_console_size();
    const size_t console_array_size = sizeof(char) * console_size.width * console_size.height;
    // Allocating screen buffer for rendering landed blocks and falling tetrominoes
    char screen[sizeof(char) * console_array_size];
    // Console Raw output initiliazation
    HANDLE console_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console_handle);
    DWORD bytes_written = 0;
    const COORD zero_coord = {
        .X = 0,
        .Y = 0,
    };
    // Clear screen with space characters
    memset(screen, ' ', console_array_size);
    WriteConsoleOutputCharacter(console_handle, screen, console_size.width * console_size.height, zero_coord, &bytes_written);

    // landed blocks with field 10x16 (10 columns and 16 rows)
    int32_t landed[LAND_SIZE] = {0};

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

    for (;;)
    {
        int32_t i = rand() % 7;
        bool left_arrow = false;
        bool right_arrow = false;
        bool up_arrow = false;
        float time = 0.5f;

        // if tetromino spawned in landed blocks game is over
        if (landed_get_value(landed, tetrominoes[i].topLeft.x, tetrominoes[i].topLeft.y) != 0)
        {
            break;
        }

        for (;;)
        {
            // input
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                goto exit;
            }
            else if (GetAsyncKeyState(VK_LEFT))
            {
                left_arrow = true;
            }
            else if (GetAsyncKeyState(VK_RIGHT))
            {
                right_arrow = true;
            }

            if (GetAsyncKeyState(VK_DOWN))
            {
                time = 0.1f;
            }
            else
            {
                time = 0.5f;
            }

            if (GetAsyncKeyState(VK_UP))
            {
                up_arrow = true;
            }
            else
            {
                up_arrow = false;
            }

            place_landed_blocks_to_screen(landed, screen, &console_size);

            QueryPerformanceCounter(&y_cooldown2);
            if (time_diff(y_cooldown1, y_cooldown2, cpu_freq) > time)
            {
                QueryPerformanceCounter(&y_cooldown1);
                ++tetrominoes[i].potentialTopLeft.y;
            }

            QueryPerformanceCounter(&x_cooldown2);
            if (left_arrow && time_diff(x_cooldown1, x_cooldown2, cpu_freq) > 0.1f)
            {
                QueryPerformanceCounter(&x_cooldown1);
                --tetrominoes[i].potentialTopLeft.x;
            }
            else if (right_arrow && time_diff(x_cooldown1, x_cooldown2, cpu_freq) > 0.1f)
            {
                QueryPerformanceCounter(&x_cooldown1);
                ++tetrominoes[i].potentialTopLeft.x;
            }
            
            QueryPerformanceCounter(&r_cooldown2);
            if (up_arrow && time_diff(r_cooldown1, r_cooldown2, cpu_freq) > 0.2f)
            {
                QueryPerformanceCounter(&r_cooldown1);
                rotate_tetromino(&tetrominoes[i], landed);
            }
            // reset arrow state
            left_arrow = false;
            right_arrow = false;
            up_arrow = false;
            place_tetromino_to_screen(&tetrominoes[i], screen, &console_size);

            if (check_colision(&tetrominoes[i], landed))
            {
                for (int32_t y = 0; y < 16; ++y)
                {
                    bool is_filled = true;
                    for (int32_t x = 0; x < 10; ++x)
                    {
                        if (landed_get_value(landed, x, y) == 0)
                        {
                            is_filled = false;
                        }
                    }
                    if (is_filled)
                    {
                        score += 100;
                        for (int32_t x = 0; x < 10; ++x)
                        {
                            landed_set_value(landed, 0, x, y);
                        }

                        for (int32_t yy = y-1; yy > 0; --yy)
                        {
                            for (int32_t x = 0; x < 10; ++x)
                            {
                                int32_t value = landed_get_value(landed, x, yy);
                                landed_set_value(landed, value, x, yy+1);
                            }
                        }
                        for (int x = 0; x < 10; ++x)
                        {
                            landed_set_value(landed, 0, x, 0);
                        }
                    }
                }

                break;
            }
            else
            {
                tetrominoes[i].topLeft = tetrominoes[i].potentialTopLeft;
            }

            // Copy screen buffer to console buffer
            WriteConsoleOutputCharacter(console_handle, screen, console_size.width * console_size.height, zero_coord, &bytes_written);
        }
    }

    // Copy last frame to console buffer
    WriteConsoleOutputCharacter(console_handle, screen, console_size.width * console_size.height, zero_coord, &bytes_written);
    memset(screen, ' ', console_array_size);
    sprintf(screen, "Score: %d", score);
    WriteConsoleOutputCharacter(console_handle, screen, console_size.width * console_size.height, zero_coord, &bytes_written);
    Sleep(2000);

exit:
#ifndef NDEBUG
    stb_leakcheck_dumpmem();
#endif // !NDEBUG
    printf("Score: %d\n", score);
    return 0;
}
