#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "buffer.h"

#include <stdbool.h>

struct Keys
{
    bool left;
    bool right;
    bool up;
    bool down;
    bool escape;
};

enum KEY_NUMS
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_ESCAPE
};

void init_console(struct Buffer *console);
void close_console(struct Buffer *console);
size_t write_to_console(struct Buffer *console);
void init_time();
float get_time_in_seconds();
float time_diff(float start, float end);
bool get_key(enum KEY_NUMS key);

#endif // PLATFORM_H_
