#pragma once

int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_initbigchar(int *big, char value);
int bc_printbigchar(int *big, int x, int y, enum colors clr1 = colors::black, enum colors clr2 = colors::white);
int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);