#include "buffer.h"

void buffer_set_value(struct Buffer *buffer, char value, int32_t x, int32_t y)
{
    buffer->buffer[buffer->width*y+x] = value;
}

char buffer_get_value(struct Buffer *buffer, int32_t x, int32_t y)
{
    return buffer->buffer[buffer->width*y+x];
}
