#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 10

char *input()
{
    char *str = (char *)malloc(sizeof(char) * INITIAL_SIZE);
    char c;

    size_t size = 0;

    while (1)
    {
        c = getchar();

        if (c == '\n')
            break;

        if (size > INITIAL_SIZE)
        {
            str = (char *)realloc(str, sizeof(char) * (size + INITIAL_SIZE));
        }

        str[size++] = c;
    }

    str = (char *)realloc(str, sizeof(char) * (size + 1));
    str[size] = '\0';

    return str;
}