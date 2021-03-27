#include "master.h"

int main()
{

    int a[2] = {};
    int b[4] = {};
    int t = -1;
    int f;

    mt_clrscr();
    bc_box(1, 1, 9, 16);
    bc_initbigchar(a, '3');
    bc_printbigchar(a, 2, 2);
    bc_initbigchar(a, '8');
    bc_setbigcharpos(a, 7, 0, 1);
    bc_printbigchar(a, 2, 10);

    mt_gotoXY(11, 0);
    printf("Bit at (7;0) is %d\n", t);
    bc_getbigcharpos(a, 7, 0, &t);

    f = open("./sav", O_RDWR);
    bc_initbigchar(a, 'F');
    bc_bigcharwrite(f, a, 1);
    bc_initbigchar(a, '2');
    bc_bigcharwrite(f, a, 1);
    lseek(f, 0, SEEK_SET);

    bc_bigcharread(f, b, 2, &t);
    printf("Read %d bytes\n", t);
    int max = t;
    int i = 0;
    while (t > 7)
    {
        bc_printbigchar(&b[i], 13, max - t);
        t -= 8;
        i += 2;
    }
    close(f);
    return 0;
}