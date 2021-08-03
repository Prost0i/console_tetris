#include "buffer.h"

#include "platform.h"

#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct termios orig_termios;

static void get_console_size(struct Buffer *console)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    console->width = w.ws_col;
    console->height = w.ws_row;
    console->size_in_bytes = console->width * console->height;
}

void init_console(struct Buffer *console)
{
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    write(STDOUT_FILENO, "\033[?1049h", 9);
    write(STDOUT_FILENO, "\033[?25l", 6);

    get_console_size(console);
}
void close_console(struct Buffer *console)
{
    (void)console;

    write(STDOUT_FILENO, "\033[?1049l", 8);
    write(STDOUT_FILENO, "\033[?25h", 6);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
size_t write_to_console(struct Buffer *console)
{
    write(STDOUT_FILENO, "\033[H", 3);
    write(STDOUT_FILENO, console->buffer, console->size_in_bytes);

    return 0;
}
void init_time()
{
}

double get_time_in_seconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return ((double)tv.tv_sec + ((double)tv.tv_usec/1000000.0));
}

double time_diff(double start, double end)
{
    return end - start;
}

void get_key(struct Keys *keys)
{
    memset(keys, 0, sizeof(struct Keys));

    char c;
    read(STDIN_FILENO, &c, 1);

    if (c == '\033')
    {
        char seq[2];

        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);

        if (seq[0] == '[')
        {
            switch (seq[1])
            {
                case 'D':
                    keys->left = true;
                    break;
                case 'C':
                    keys->right = true;
                    break;
                case 'A':
                    keys->up = true;
                    break;
                case 'B':
                    keys->down = true;
                    break;
                case '\0':
                    keys->escape = true;
                    break;
            }
        }
        else if (seq[0] == '\0')
        {
            keys->escape = true;
        }
    }
}
