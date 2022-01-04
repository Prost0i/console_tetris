#include "buffer.h"
#include "tetromino.h"
#include "draw.h"
#include "landed.h"
#include "checks.h"
#include "platform.h"
#include "gamelogic.h"

#include "buffer.c"
#include "tetromino.c"
#include "draw.c"
#include "landed.c"
#include "checks.c"
#include "gamelogic.c"

#ifdef _WIN32
#include "win32_platform.c"
#endif

#ifdef __unix__
#include "posix_platform.c"
#endif

#include <stdio.h>
#include <stdint.h>
#include <time.h>

int main()
{
    // Allocating screen buffer for rendering landed blocks and falling tetrominoes
    char *screen = malloc(sizeof(char)*1000*1000);
    
    // Seed random
    srand((uint32_t)time(NULL));
    
    struct Buffer console = {
        .buffer = screen,
        .width = 0,
        .height = 0,
        .size_in_bytes = 0
    };
    init_console(&console);
    init_time();
    
    int32_t score = mainloop(&console);
    
    close_console(&console);
    
    printf("Score: %d\n", score);
    return 0;
}
