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
#define NM 2 //not enough memory
#define EC 3 //no such command
#define OF 4 //overflow
#define IC 5 //ignore clock
#define ZR 6 //divide by zero

enum class commands
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
    NOT = 51,
    AND = 52,
    OR = 53,
    XOR = 54,
    JNS = 55,
    JC = 56,
    JNC = 57,
    CHL = 60,
    SHR = 61,
    RCL = 62,
    RCR = 63,
    NEG = 64,
    ADDC = 65,
    SUBC = 66,
    LOGLC = 67,
    LOGRC = 68,
    RCCL = 69,
    RCCR = 70,
    MOVA = 71,
    MOVR = 72,
    MOVCA = 73,
    MOVCR = 74,
    ADDCE = 75,
    SUBCE = 76
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
