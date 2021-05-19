#include "master.h"
#include "l2.h"

int mt_clrscr(void)
{
    printf("\E[H\E[2J");
    return 0;
}

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

int mt_setbgcolor(enum colors color)
{
    printf("\E[4%dm", color);
    return 0;
}

int mt_setfgcolor(enum colors color)
{
    printf("\E[3%dm", color);
    return 0;
}