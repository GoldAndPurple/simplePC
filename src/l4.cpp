#include "master.h"
#include "l4.h"

int rk_readkey(enum keys *key)
{
    char input[5];
    struct termios initial;

    if (tcgetattr(0, &initial) != 0)
    {
        return -1;
    }
    if (rk_mytermregime(0, 1, 1, 0, 0) != 0)
    {
        return -1;
    }

    read(0, input, 5);

    if (input[0] == 'q')
    {
        *key = keys::QUIT;
    }
    else if (input[0] == 'l')
    {
        *key = keys::LOAD;
    }
    else if (input[0] == 's')
    {
        *key = keys::SAVE;
    }
    else if (input[0] == 'r')
    {
        *key = keys::RUN;
    }
    else if (input[0] == 't')
    {
        *key = keys::STEP;
    }
    else if (input[0] == 'i')
    {
        *key = keys::RESET;
    }
    else if (input[0] == 'e')
    {
        *key = keys::ENTER;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == '1') && (input[3] == '5'))
    {
        *key = keys::F5;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == '1') && (input[3] == '7'))
    {
        *key = keys::F6;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == 'A'))
    {
        *key = keys::UP;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == 'B'))
    {
        *key = keys::DOWN;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == 'C'))
    {
        *key = keys::RIGHT;
    }
    else if ((input[0] == '\033') && (input[1] == '[') && (input[2] == 'D'))
    {
        *key = keys::LEFT;
    }

    if (!tcsetattr(0, TCSANOW, &initial))
    {
        return -1;
    }

    return 0;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios options;

    if (tcgetattr(0, &options) != 0)
    {
        return -1;
    }

    if (regime == 1)
    {
        options.c_lflag |= ICANON;
    }
    else if (regime == 0)
    {
        options.c_lflag &= ~ICANON;

        options.c_cc[VTIME] = vtime;
        options.c_cc[VMIN] = vmin;

        if (echo == 1)
        {
            options.c_lflag |= ECHO;
        }
        else if (echo == 0)
        {
            options.c_lflag &= ~ECHO;
        }
        if (sigint == 1)
        {
            options.c_lflag |= ISIG;
        }
        else if (sigint == 0)
        {
            options.c_lflag &= ~ISIG;
        }
    }

    if (tcsetattr(0, TCSANOW, &options) != 0)
    {
        return -1;
    }
    return 0;
}

int rk_mytermsave(void)
{
    struct termios terminal;
    FILE *data;

    if (!tcgetattr(0, &terminal))
    {
        return -1;
    }

    if ((data = fopen("terminal.data", "wb")) == NULL)
    {
        return -1;
    }

    fwrite(&terminal, sizeof(terminal), 1, data);
    fclose(data);

    return 0;
}

int rk_mytermrestore(void)
{
    struct termios terminal;
    FILE *data;

    if ((data = fopen("terminal.data", "wb")) == NULL)
    {
        return -1;
    }

    if (fread(&terminal, sizeof(terminal), 1, data) > 0)
    {
        if (!tcsetattr(0, TCSAFLUSH, &terminal))
        {
            return -1;
        }
        else
        {
            fclose(data);
        }
    }

    return 0;
}