#include "master.h"
#include "gui.h"
#include "l1.h"
#include "l2.h"
#include "l3.h"
#include "l4.h"

enum keys key;
struct itimerval nval;
int accumulator = 0;
int counter = 0;
int com;
int op;
int cursor = 0;
int temp = 0;

int ALU(int command, int operand)
{
    int value;

    sc_memoryGet(operand, &value);
    switch (command)
    {
    case 30:
        //printf("ADD\n");
        if (9999 < value + accumulator)
        {
            sc_regSet(OF, 1);
            sc_regSet(IC, 1);
            cursor = counter = 0;
            break;
        }
        else
        {
            accumulator += value;
            counter++;
        }
        break;
    case 31:
        //printf("SUB\n");
        accumulator -= value;
        counter++;
        break;
    case 32:
        //printf("DIVIDE\n");
        if (value == 0)
        {
            sc_regSet(ZR, 1);
            sc_regSet(IC, 1);
            cursor = counter = 0;
            break;
        }
        else
        {
            accumulator /= value;
            counter++;
        }
        break;
    case 33:
        //printf("MUL\n");
        if ((9999 / accumulator + 1) < value)
        {
            sc_regSet(OF, 1);
            sc_regSet(IC, 1);
            cursor = counter = 0;
            break;
        }
        else
        {
            accumulator *= value;
            counter++;
        }
        break;
    }
    return 0;
}

int CU()
{
    int value = 0;

    sc_memoryGet(counter, &value);

    if (sc_commandDecode(value, &com, &op))
    {
        return -1;
    }
    else
    {
        if ((com >= 30) && (com <= 33))
        {
            sc_regSet(EC, 0);
            ALU(com, op);
        }
        else
        {
            sc_regSet(EC, 0);
            enum coms command = (coms)com;
            switch (command)
            {
                int t;
            case coms::READ:
                printf("READ:\n");
                scanf("%d", &t);
                if (t > 9999)
                {
                    sc_regSet(OF, 1);
                    sc_regSet(IC, 1);
                    cursor = counter = 0;
                }
                else
                {
                    sc_memorySet(op, t);
                    counter++;
                }
                break;
            case coms::WRITE:
                printf("WRITE:\n");
                sc_memoryGet(op, &t);
                printf("\t%d\n", t);
                counter++;
                break;
            case coms::LOAD:
                sc_memoryGet(op, &t);
                accumulator = t;
                counter++;
                break;
            case coms::STORE:
                sc_memorySet(op, accumulator);
                counter++;
                break;
            case coms::JUMP:
                counter = op;
                break;
            case coms::JNEG:
                if (accumulator < 0)
                {
                    counter = op;
                }
                else
                {
                    counter++;
                }
                break;
            case coms::JZ:
                if (accumulator == 0)
                {
                    counter = op;
                }
                else
                {
                    counter++;
                }
                break;
            case coms::JNZ:
                if (accumulator == 0)
                {
                    counter++;
                }
                else
                {
                    counter = op;
                }
                break;
            case coms::HALT:
                sc_regSet(IC, 1);
                break;
            case coms::MOVR:
                sc_memoryGet(accumulator, &t);
                sc_memorySet(op, t);
                counter++;
                break;
            case coms::CONST:
                sc_memorySet(99, op);
                counter++;
                break;
            default:
                sc_regSet(EC, 1);
                sc_regSet(IC, 1);
                break;
            }
        }

        return 0;
    }
    return 0;
}

void print_memory()
{
    mt_gotoXY(2, 28);
    printf("memory");
    mt_gotoXY(2, 65);
    printf("accumulator");
    mt_gotoXY(3, 65);
    printf("%+03d", accumulator);
    mt_gotoXY(5, 65);
    printf("instructionCounter");
    mt_gotoXY(6, 65);
    printf("%+03d", counter);
    mt_gotoXY(8, 65);
    printf("operation");
    mt_gotoXY(9, 65);
    printf("%02d : %02d", com, op);

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
        if (temp > 9999)
        {
            printf("+%04x ", temp);
        }
        else
        {
            printf("%+05d ", temp);
        }
    }
}

void print_flags()
{
    int fl = 0;
    mt_gotoXY(11, 65);
    printf("flags");
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

void print_keywords()
{
    mt_gotoXY(14, 56);
    printf("keys:");
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
    mt_gotoXY(22, 56);
    printf("q - quit");
}

void print_frames()
{
    bc_box(2, 1, 11, 60);
    bc_box(2, 63, 2, 20);
    bc_box(5, 63, 2, 20);
    bc_box(8, 63, 2, 20);
    bc_box(11, 63, 2, 20);
    bc_box(14, 1, 9, 52);
    bc_box(14, 55, 9, 28);
}

void print_current_inst()
{
    int value;
    int bigchar[2];
    char char_symbol[8];

    sc_memoryGet(cursor, &value);
    sprintf(char_symbol, "+%04x", value);

    for (int i = 0; i < 5; i++)
    {
        bc_initbigchar(bigchar, char_symbol[i]);
        bc_printbigchar(bigchar, 15, 6 + (i * 9), colors::black, colors::white);
    }
}

void print_gui()
{
    print_frames();
    print_keywords();
    print_flags();
    print_memory();
    print_current_inst();
    printf("\nInput/Output\n");
}

void settimer(struct itimerval *nval)
{
    nval->it_interval.tv_sec = 0;
    nval->it_interval.tv_usec = 0;
    nval->it_value.tv_sec = 0;
    nval->it_value.tv_usec = 100000;
}

void timer(int sig)
{
    print_gui();
    if (CU() == -1)
    {
        sc_regSet(IC, 1);
    }
    else
    {
        cursor = counter;
    }
}

void reset(int sig)
{
    sc_memoryInit();
    sc_regInit();
    sc_regSet(IC, 1);
    cursor = counter = accumulator = 0;
}