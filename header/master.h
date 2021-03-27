#ifndef BASIC_PC
#define BASIC_PC

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

enum class colors
{
    red = 1,
    green = 2,
    yellow = 3,
    blue = 4,
    white = 7,
    black = 8,
};

//lab 1
int sc_regInit(void);
int sc_regSet(int reg, int value);
int sc_regGet(int reg, int *value);
int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);

//lab 2
int mt_clrscr(void);
int mt_gotoXY(int x, int y);
int mt_getscreensize(int *rows, int *cols);
int mt_setbgcolor(enum colors color);
int mt_setfgcolor(enum colors color);

//lab 3
int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_initbigchar(int *big, char value);
int bc_printbigchar(int *big, int x, int y, enum colors clr1 = colors::black, enum colors clr2 = colors::white);
int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);

#endif