#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>

#define N 100 //allocated memory

#define OB 1 //out of bounds
#define ZR 2 //divide by zero
#define EC 3 //no such command
#define OF 4 //overflow
#define IC 5 //ignore clock
#define NM 6 //not enough memory

enum class coms
{
    READ = 10,
    WRITE = 11,
    LOAD = 20,
    STORE = 21,
    ADD = 30,
    SUB = 31,
    DIVIDE = 32,
    MUL = 33,
    JUMP = 40,
    JNEG = 41,
    JZ = 42,
    HALT = 43,
    MOVR = 72,
};
enum class colors
{
    black = 0,
    red = 1,
    green = 2,
    yellow = 3,
    blue = 4,
    white = 7,
    def = 9,
};
enum class keys
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LOAD,
    SAVE,
    RUN,
    STEP,
    RESET,
    F5,
    F6,
    QUIT,
    ENTER,
};
