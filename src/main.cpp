#include "master.h"
#include "l1.h"
#include "l2.h"
#include "l3.h"
#include "l4.h"
#include "gui.h"
#include "lang.h"

int main(void)
{
    sc_memoryInit();
    sc_regInit();

    sc_regSet(IC, 1);
    int x, y, z, acc, inst, value, code;
    char f;
    char t;

    signal(SIGALRM, timer);
    signal(SIGUSR1, reset);

    settimer(&nval);

    while (key != keys::QUIT)
    {
        mt_clrscr();
        print_gui();
        sc_regGet(IC, &value);
        if (value)
        {
            rk_readkey(&key);
            switch (key)
            {
            case keys::SAVE:
                printf("SAVE: Input filename\n");
                scanf("%s", &f);
                sc_memorySave(&f);
                break;
            case keys::LOAD:
                printf("LOAD: Input filename\n");
                scanf("%s", &f);
                sc_memoryLoad(&f);
                break;
            case keys::F5:
                printf("Input accumulator value\n");
                scanf("%d", &acc);
                if (acc > 0x7FFF)
                {
                    sc_regSet(OF, 1);
                }
                else
                {
                    sc_regSet(OF, 0);
                    accumulator = acc;
                }
                break;
            case keys::F6:
                printf("Input counter value\n");
                scanf("%d", &inst);
                if ((inst < 0) || (inst > N - 1))
                {
                    sc_regSet(NM, 1);
                }
                else
                {
                    sc_regSet(NM, 0);
                    counter = inst;
                }
                break;
            case keys::UP:
                cursor -= 10;
                if (cursor < 0)
                {
                    cursor += N;
                }
                break;
            case keys::DOWN:
                cursor += 10;
                if (cursor > N - 1)
                {
                    cursor -= N;
                }
                break;
            case keys::LEFT:
                cursor--;
                if (cursor < 0)
                {
                    cursor = N - 1;
                }
                break;
            case keys::RIGHT:
                cursor++;
                if (cursor > N - 1)
                {
                    cursor = 0;
                }
                break;
            case keys::RUN:
                sc_regInit();
                cursor = 0;
                break;
            case keys::ENTER:
                printf("1 - Manual\n2 - Basic\n3 - Assembler\n");
                scanf("%d", &z);
                if (z == 1)
                {
                    printf("Command\n");
                    scanf("%d", &x);
                    printf("Operand\n");
                    scanf("%d", &y);
                    if (x == 0)
                    {
                        sc_memorySet(cursor, y);
                    }
                    else if (sc_commandEncode(x, y, &code) == 0)
                    {
                        sc_memorySet(cursor, code);
                    }
                }
                else if (z == 2)
                {
                    printf("Input filename\n");
                    scanf("%s", &t);
                    basic(&t);
                }
                else if (z == 3)
                {
                    printf("Input filename\n");
                    scanf("%s", &t);
                    translate(&t);
                }
                break;
            case keys::STEP:
                CU();
                break;
            case keys::RESET:
                raise(SIGUSR1);
                break;
            default:
                break;
            }
        }
        else
        {
            setitimer(ITIMER_REAL, &nval, NULL);
            pause();
        }
    }
    mt_setbgcolor(colors::black);
    mt_setfgcolor(colors::white);
    return 0;
}