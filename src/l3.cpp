#include "master.h"
#include "l3.h"
#include "l2.h"

#define BC_ENTER "\E(0"
#define BC_EXIT "\E(B"

int bc_initbigchar(int *big, char value)
{
    switch (value)
    {
    case '-':
        big[0] = 4278190080;
        big[1] = 255;
        break;

    case '+':
        big[0] = 4279769112;
        big[1] = 404232447;
        break;

    case '0':
        big[0] = 2172748287;
        big[1] = 4286677377;
        break;

    case '1':
        big[0] = 1617721456;
        big[1] = 1616928864;
        break;

    case '2':
        big[0] = 1617181720;
        big[1] = 4282260536;
        break;

    case '3':
        big[0] = 541073948;
        big[1] = 472006712;
        break;

    case '4':
        big[0] = 606348324;
        big[1] = 538976316;
        break;

    case '5':
        big[0] = 470025276;
        big[1] = 405020704;
        break;

    case '6':
        big[0] = 436347928;
        big[1] = 405021222;
        break;

    case '7':
        big[0] = 811614334;
        big[1] = 33818748;
        break;

    case '8':
        big[0] = 405021720;
        big[1] = 405021720;
        break;

    case '9':
        big[0] = 606348312;
        big[1] = 405020728;
        break;

    case 'A':
        big[0] = 0x3C24243C;
        big[1] = 0x24242424;
        break;

    case 'B':
        big[0] = 0xE12221E;
        big[1] = 0x1E22120E;
        break;

    case 'C':
        big[0] = 0x2022418;
        big[1] = 0x18240202;
        break;

    case 'D':
        big[0] = 0x2424140C;
        big[1] = 0xC142424;
        break;

    case 'E':
        big[0] = 0x7E027E7E;
        big[1] = 0x7E7E027E;
        break;

    case 'F':
        big[0] = 0x3C047C7C;
        big[1] = 0x404043C;
        break;

    default:
        break;
    }

    return 0;
}

int bc_printA(char *str)
{
    if (!str)
    {
        return -1;
    }
    printf("%s", BC_ENTER);
    printf("%s", str);
    printf("%s", BC_EXIT);
    return 0;
}

int bc_box(int x1, int y1, int x2, int y2)
{
    printf("%s", BC_ENTER);
    mt_gotoXY(x1, y1);
    printf("l");
    for (int i = (x1 + 1); i <= (x1 + y2); i++)
    {
        printf("q");
    }
    printf("k");
    mt_gotoXY(x1, y1);
    for (int i = (x1 + 1); i < (x1 + x2); i++)
    {
        mt_gotoXY(i, y1 + y2 + 1);
        printf("x");
        mt_gotoXY(i, y1);
        printf("x");
    }
    mt_gotoXY(x1 + x2, y1);
    printf("m");
    for (int i = (x1 + 1); i <= (x1 + y2); i++)
    {
        printf("q");
    }
    printf("j");
    printf("%s", BC_EXIT);
    return 0;
}

int bc_printbigchar(int *big, int x, int y, enum colors clr1, enum colors clr2)
{
    mt_gotoXY(x, y);

    int bit;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bit = (big[i >> 2] >> ((i % 4) * 8 + j)) & 0x1;
            if (bit)
            {
                mt_setbgcolor(clr2);
                mt_setfgcolor(clr1);
                printf(" ");
            }
            else
            {
                mt_setbgcolor(clr1);
                mt_setfgcolor(clr2);
                printf(" ");
            }
        }
        mt_gotoXY(x + (i + 1), y);
    }
    mt_setbgcolor(colors::black);
    mt_setfgcolor(colors::white);
    return 0;
}

int bc_setbigcharpos(int *big, int x, int y, int value)
{
    if ((x < 0) || (x > 7) || (y < 0) || (y > 7))
    {
        return -1;
    }
    if (value)
    {
        big[x / 4] |= (1 << ((x % 4) * 8 + y));
    }
    else
    {
        big[x / 4] &= (~(1 << ((x % 4) * 8 + y)));
    }
    return 0;
}

int bc_getbigcharpos(int *big, int x, int y, int *value)
{
    if ((x < 0) || (x > 7) || (y < 0) || (y > 7))
    {
        return -1;
    }

    *value = (big[x / 4] >> ((x % 4) * 8 + y)) & 0x1;
    return 0;
}

int bc_bigcharwrite(int fd, int *big, int count)
{
    write(fd, big, count * sizeof(int) * 2);
    return 0;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
    *count = read(fd, big, need_count * sizeof(int) * 2);
    if (*count)
    {
        return 0;
    }
    return -1;
}