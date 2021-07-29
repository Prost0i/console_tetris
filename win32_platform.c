#include <Windows.h>

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

float get_time_in_seconds()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return (float)result.QuadPart;
}

float time_diff(float start, float end)
{
    return ((end - start) / (float)win32_time_frequency.QuadPart);
}

bool get_key(enum KEY_NUMS key)
{
    switch(key)
    {
        case KEY_LEFT:
            return GetAsyncKeyState(VK_LEFT);
        case KEY_RIGHT:
            return GetAsyncKeyState(VK_RIGHT);
        case KEY_UP:
            return GetAsyncKeyState(VK_UP);
        case KEY_DOWN:
            return GetAsyncKeyState(VK_DOWN);
        case KEY_ESCAPE:
            return GetAsyncKeyState(VK_ESCAPE);
        default:
            return false;
    }
}
