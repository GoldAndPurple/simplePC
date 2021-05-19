#include "master.h"
#include "gui.h"
#include "l1.h"
#include "l2.h"
#include "l3.h"
#include "l4.h"

enum keys key;
struct itimerval nval;
int accumulator;
int inst_counter;
int com;
int op;
int cursor;
int temp;

int alu(int command, int operand)
{
    int value;

    sc_memoryGet(operand, &value);
    switch (command)
    {
    case 0x1E:
        //printf("ADD\n");
        if ((0x7FFF - accumulator) < value)
        {
            sc_regSet(OF, 1);
            sc_regSet(IC, 1);
            cursor = inst_counter = 0;
            break;
        }
        else
        {
            accumulator += value;
            inst_counter++;
        }
        break;
    case 0x1F:
        //printf("SUB\n");
        accumulator -= value;
        inst_counter++;
        break;
    case 0x20:
        //printf("DIVIDE\n");
        if (value == 0)
        {
            sc_regSet(ZR, 1);
            sc_regSet(IC, 1);
            cursor = inst_counter = 0;
            break;
        }
        else
        {
            accumulator /= value;
            inst_counter++;
        }
        break;
    case 0x21:
        //printf("MUL\n");
        if ((0x777F / accumulator + 1) < value)
        {
            sc_regSet(OF, 1);
            sc_regSet(IC, 1);
            cursor = inst_counter = 0;
            break;
        }
        else
        {
            accumulator *= value;
            inst_counter++;
        }
        break;
    }
    return 0;
}

int cu()
{
    int value = 0;
    int result;

    sc_memoryGet(inst_counter, &value);
    result = sc_commandDecode(value, &com, &op);

    if (result != 0)
    {
        sc_regSet(EC, 1);
        return -1;
    }
    else
    {
        if ((com == 0x1E) || (com == 0x1F) || (com == 0x20) || (com == 0x21))
        {
            alu(com, op);
        }
        else
        {
            switch (com)
            {
                int x, y;
            case 0xA:
                printf("READ\n");
                scanf("%d", &y);
                if (y > 0x7FFF)
                {
                    sc_regSet(OF, 1);
                    sc_regSet(IC, 1);
                    cursor = inst_counter = 0;
                }
                else
                {
                    sc_memorySet(op, y);
                    inst_counter++;
                }
                break;
            case 0xB:
                printf("WRITE\n");
                sc_memoryGet(op, &y);
                printf("\n%d", y);
                inst_counter++;
                break;
            case 0x14:
                printf("LOAD\n");
                sc_memoryGet(op, &y);
                accumulator = y;
                inst_counter++;
                break;
            case 0x15:
                printf("STORE\n");
                sc_memorySet(op, accumulator);
                inst_counter++;
                break;
            case 0x28:
                printf("JUMP\n");
                inst_counter = op;
                break;
            case 0x29:
                printf("JNEG\n");
                if (accumulator < 0)
                {
                    inst_counter = op;
                }
                else
                {
                    inst_counter++;
                }
                break;
            case 0x2A:
                printf("JZ\n");
                if (accumulator == 0)
                {
                    inst_counter = op;
                }
                else
                {
                    inst_counter++;
                }
                break;
            case 0x2B:
                printf("HALT\n");
                sc_regSet(IC, 1);
                cursor = inst_counter = 0;
                break;
            case 0x40:
                printf("NEG\n");
                sc_memoryGet(op, &y);
                accumulator = -y;
                inst_counter++;
                break;
            case 0x42:
                printf("SUBC\n");
                sc_memoryGet(op, &x);
                sc_memoryGet(accumulator, &y);
                accumulator = (x - y);
                inst_counter++;
                break;
            case 0x47:
                printf("SUBC\n");
                sc_memoryGet(op, &x);
                sc_memorySet(accumulator, x);
                inst_counter++;
                break;
            }
        }

        return 0;
    }
}

void memoryShow()
{
    for (int i = 0; i < N; i++)
    {
        if (i == cursor)
        {
            mt_setbgcolor(colors::white);
            mt_setfgcolor(colors::black);
        }
        else
        {
            mt_setbgcolor(colors::black);
            mt_setfgcolor(colors::white);
        }
        if (i % 10 == 0)
        {
            mt_gotoXY((i / 10) + 3, 2);
        }
        sc_memoryGet(i, &temp);
        printf("+%04X ", temp);
    }
}

void CPUshow()
{
    mt_gotoXY(2, 28);
    printf("memory");
    mt_gotoXY(2, 65);
    printf("accumulator");
    mt_gotoXY(3, 65);
    printf("+%04X", accumulator);
    mt_gotoXY(5, 65);
    printf("instructionCounter");
    mt_gotoXY(6, 65);
    printf("+%04X", inst_counter);
    mt_gotoXY(8, 65);
    printf("operation");
    mt_gotoXY(9, 65);
    printf("+%02d : %02d", com, op);
}

void flagShow()
{
    int fl = 0;
    mt_gotoXY(11, 65);
    printf("flags %d", flags);
    mt_gotoXY(12, 65);
    sc_regGet(OF, &fl);
    if (fl)
    {
        printf("OF");
    }
    else
    {
        printf(" ");
    }
    sc_regGet(NM, &fl);
    if (fl)
    {
        printf("NM ");
    }
    else
    {
        printf(" ");
    }
    sc_regGet(ZR, &fl);
    if (fl)
    {
        printf("ZR ");
    }
    else
    {
        printf(" ");
    }
    sc_regGet(IC, &fl);
    if (fl)
    {
        printf("IC ");
    }
    else
    {
        printf(" ");
    }
    sc_regGet(EC, &fl);
    if (fl)
    {
        printf("EC");
    }
    else
    {
        printf(" ");
    }
}

void keysShow()
{
    mt_gotoXY(14, 56);
    printf("Keys:");
    mt_gotoXY(15, 56);
    printf("l - load");
    mt_gotoXY(16, 56);
    printf("s - save");
    mt_gotoXY(17, 56);
    printf("r - run");
    mt_gotoXY(18, 56);
    printf("t - step");
    mt_gotoXY(19, 56);
    printf("i - reset");
    mt_gotoXY(20, 56);
    printf("f5 - accumulator");
    mt_gotoXY(21, 56);
    printf("f6 - instructionCounter");
}

void printBoxes()
{
    bc_box(2, 1, 11, 60);
    bc_box(2, 63, 2, 20);
    bc_box(5, 63, 2, 20);
    bc_box(8, 63, 2, 20);
    bc_box(11, 63, 2, 20);
    bc_box(14, 1, 9, 52);
    bc_box(14, 55, 9, 28);
}

void printBigChars()
{
    int value;
    int bigchar[2];
    char char_symbol[8];

    sc_memoryGet(inst_counter, &value);
    if (value >> 14)
    {
        sprintf(char_symbol, "-%04X", value);
    }
    else
    {
        sprintf(char_symbol, "+%04X", value);
    }

    for (int i = 0; i < 5; i++)
    {
        bc_initbigchar(bigchar, char_symbol[i]);
        bc_printbigchar(bigchar, 15, 6 + (i * 9), colors::white, colors::def);
    }
}

void console()
{
    printBoxes();
    keysShow();
    flagShow();
    CPUshow();
    memoryShow();
    printBigChars();
    printf("\nInput/Output\n");
}

void settimer(struct itimerval *nval)
{
    nval->it_interval.tv_sec = 0;
    nval->it_interval.tv_usec = 0;
    nval->it_value.tv_sec = 0;
    nval->it_value.tv_usec = 100000;
}

void timer()
{
    if (cu() == -1)
    {
        sc_regSet(IC, 1);
        cursor = inst_counter = 0;
    }
    else
    {
        console();
        cursor = inst_counter;
    }
}

void reset()
{
    sc_memoryInit();
    sc_regInit();
    sc_regSet(IC, 1);
    cursor = inst_counter = accumulator = 0;
}