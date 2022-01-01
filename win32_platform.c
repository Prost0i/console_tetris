#include <Windows.h>
#include <string.h>

#include "platform.h"

static void get_console_size(struct Buffer *console)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    console->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    console->size_in_bytes = console->width * console->height;
}

HANDLE win32_console_handle;
LARGE_INTEGER win32_time_frequency;

void init_console(struct Buffer *console)
{
    // Console Raw output initiliazation
    win32_console_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(win32_console_handle);
    // Getting console dimensions and size in bytes
    get_console_size(console);
}
void close_console(struct Buffer *console)
{
    (void)console;
    CloseHandle(win32_console_handle);
}

size_t write_to_console(struct Buffer *console)
{
    // Coordinates of upper left corner of console
    const COORD zero_coord = { .X = 0, .Y = 0 };
    
    DWORD bytes_written = 0;
    WriteConsoleOutputCharacter(win32_console_handle, console->buffer, console->size_in_bytes, zero_coord, &bytes_written);
    return (size_t)bytes_written;
}

void init_time()
{
    QueryPerformanceFrequency(&win32_time_frequency);
}

double get_time_in_seconds()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return (double)result.QuadPart;
}

double time_diff(double start, double end)
{
    return ((end - start) / (double)win32_time_frequency.QuadPart);
}

void get_key(struct Keys *keys)
{
    keys->left = GetAsyncKeyState(VK_LEFT);
    keys->right = GetAsyncKeyState(VK_RIGHT);
    keys->up = GetAsyncKeyState(VK_UP);
    keys->down = GetAsyncKeyState(VK_DOWN);
    keys->escape = GetAsyncKeyState(VK_ESCAPE);
}
