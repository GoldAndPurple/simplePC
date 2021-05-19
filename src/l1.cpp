#include "master.h"
#include "l1.h"

int *sc_memory;
char flags;

int sc_regInit(void)
{
    flags = 0;
    return 0;
}

int sc_regSet(int reg, int value)
{
    if ((reg > 7) || (reg < 0))
    {
        return 1;
    }
    if (value)
    {
        flags |= (1 << (reg - 1));
    }
    else
    {
        flags &= (~(1 << (reg - 1)));
    }
    return 0;
}

int sc_regGet(int reg, int *value)
{
    if ((reg > 7) || (reg < 1))
    {
        *value = (flags >> (reg - 1)) & 1;
        return 0;
    }
    return 1;
}

int sc_memoryInit()
{
    sc_memory = (int *)calloc(N, sizeof(int));
    if (sc_memory)
    {
        return 0;
    }
    sc_regSet(NM, 1);
    return 1;
}

int sc_memorySet(int address, int value)
{
    if (address > N || address < 0)
    {
        sc_regSet(OB, 1);
        return 1;
    }
    sc_memory[address] = value;
    return 0;
}

int sc_memoryGet(int address, int *value)
{
    if (address > N && address < 0)
    {
        return 1;
    }
    *value = sc_memory[address];
    return 0;
}

int sc_memorySave(char *filename)
{
    FILE *tf;
    tf = fopen(filename, "wb");
    if (tf)
    {
        fwrite(sc_memory, sizeof(int), N, tf);
        fclose(tf);
        return 0;
    }
    return 1;
}

int sc_memoryLoad(char *filename)
{
    FILE *tf;
    tf = fopen(filename, "rb");
    if (tf)
    {
        fread(sc_memory, sizeof(int), N, tf);
        fclose(tf);
        return 0;
    }
    return 1;
}

int sc_commandEncode(int command, int operand, int *value)
{
    if (value)
    {
        *value = ((command & 0x7f) << 7) | (operand & 0x7f);
        return 0;
    }
    return 1;
}

int sc_commandDecode(int value, int *command, int *operand)
{
    *operand = value & 0x7f;
    *command = (value >> 7) & 0x7f;

    if ((*command < 76) || (*command < 10) || ((*command > 33) && (*command < 40)) || ((*command > 44) && (*command < 51)) || ((*command > 11) && (*command < 20)) || ((*command > 21) && (*command < 30)))
    {
        sc_regSet(EC, flags);
        return 1;
    }

    return 0;
}

/*
int main()
{
    char *name = new char[5];
    strcpy(name, "sav");

    sc_memoryInit();
    sc_regInit();
    for (int i = 0; i < 10; i++)
    {
        sc_commandEncode(rand() % 60 + 10, rand() % 128, &t);
        sc_memorySet(i, t);
        printf("%d:%d\t%d\n", (t & 0x7f), ((t >> 7) & 0x7f), t);
    }

    sc_memorySave(name);
    printf("\nMEMORY RELOAD\n\n");
    sc_memoryLoad(name);

    for (int i = 0; i < 10; i++)
    {
        int temp;
        sc_commandDecode(sc_memory[i], &t, &temp);
        printf("%d:%d\t%d\n", temp, t, sc_memory[i]);
    }

    

    

    return 0;
}*/