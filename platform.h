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
    bool y;
    bool n;
};

void init_console(struct Buffer *console);
void close_console(struct Buffer *console);
size_t write_to_console(struct Buffer *console);
void init_time();
double get_time_in_seconds();
double time_diff(double start, double end);
void get_key(struct Keys *keys);
void platform_sleep(uint32_t millis);

#endif // PLATFORM_H_
