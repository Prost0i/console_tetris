#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>

struct Buffer
{
    char *buffer;
    int32_t width;
    int32_t height;
    int32_t size_in_bytes;
};

void buffer_set_value(struct Buffer *buffer, char value, int32_t x, int32_t y);
char buffer_get_value(struct Buffer *buffer, int32_t x, int32_t y);

#endif // BUFFER_H_
