#include "master.h"

// clear_screen=\E[H\E[2J
int mt_clrscr(void)
{
    printf("\E[H\E[2J");
    return 0;
}

// cursor_address=\E[%p1%d;%p2%dH
int mt_gotoXY(int x, int y)
{
    printf("\E[%d;%dH", x, y);
    return 0;
}

int mt_getscreensize(int *rows, int *cols)
{
    struct winsize winsize;
    //stdout - size request - var
    ioctl(1, TIOCGWINSZ, &winsize);
    *rows = winsize.ws_row;
    *cols = winsize.ws_col;
    return 0;
}

// set_a_background=\E[4%p1%dm
int mt_setbgcolor(enum colors color)
{
    printf("\E[4%dm", color);
    return 0;
}

// set_a_foreground=\E[3%p1%dm
int mt_setfgcolor(enum colors color)
{
    printf("\E[3%dm", color);
    return 0;
}