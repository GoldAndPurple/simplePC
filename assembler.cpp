#include "master.h"
#include "l1.h"

FILE *input = NULL;

void load_file(char *filename)
{
    if ((input = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot open file: no such file\n");
        exit(EXIT_FAILURE);
    }
    return;
}

void translating(char *filename)
{
    int i,
        flg = 0;
    for (i = 0; !feof(input); i++)
    {
        int intstr = 0;
        if (!fscanf(input, "%d", &intstr))
        {
            flg = 1;
            break;
        }
        char command[10] = "\0";
        int cmd = 0,
            operand = 0,
            value = 0;
        fscanf(input, "%s", command);
        if (feof(input))
            break;
        if (!strcmp(command, "READ"))
            cmd = (int)coms::READ;
        else if (!strcmp(command, "WRITE"))
            cmd = (int)coms::WRITE;
        else if (!strcmp(command, "LOAD"))
            cmd = (int)coms::LOAD;
        else if (!strcmp(command, "STORE"))
            cmd = (int)coms::STORE;
        else if (!strcmp(command, "ADD"))
            cmd = (int)coms::ADD;
        else if (!strcmp(command, "SUB"))
            cmd = (int)coms::SUB;
        else if (!strcmp(command, "DIVIDE"))
            cmd = (int)coms::DIVIDE;
        else if (!strcmp(command, "MUL"))
            cmd = (int)coms::MUL;
        else if (!strcmp(command, "JUMP"))
            cmd = (int)coms::JUMP;
        else if (!strcmp(command, "JNEG"))
            cmd = (int)coms::JNEG;
        else if (!strcmp(command, "JZ"))
            cmd = (int)coms::JZ;
        else if (!strcmp(command, "HALT"))
            cmd = (int)coms::HALT;
        else if (!strcmp(command, "JNZ"))
            cmd = (int)coms::JNZ;
        else if (!strcmp(command, "CONST"))
            cmd = (int)coms::CONST;
        else if (!strcmp(command, "="))
            sc_memorySet(intstr, operand);
        else if (atoi(command) || command[0] == '0')
        {
            sc_memorySet(intstr, atoi(command));
            continue;
        }
        else
        {
            flg = 2;
            break;
        }

        if (!fscanf(input, "%d", &operand))
        {
            flg = 3;
            break;
        }

        if (sc_commandEncode(cmd, operand, &value))
        {
            flg = 4;
            break;
        }
        sc_memorySet(intstr, value);
    }
    if (!flg)
        sc_memorySave(filename);
    if (flg == 1)
        fprintf(stderr, "line %d: expected num of line\n", i);
    if (flg == 2)
        fprintf(stderr, "line %d: wrong command\n", i);
    if (flg == 3)
        fprintf(stderr, "line %d: wrong operand\n", i);
    if (flg == 4)
        fprintf(stderr, "line %d: wrong command or operand\n", i);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s input.sa output.o\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    sc_memoryInit();
    load_file(argv[1]);
    translating(argv[2]);
    printf("Success\n");
    return 0;
}