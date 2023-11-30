#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>

#define INITIAL_SIZE 10

char *input()
{
    char *str = (char *)malloc(sizeof(char) * INITIAL_SIZE);
    char c;

    size_t size = 0;

    while (true)
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

bool file_exists(char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

void clear_buffer(char *input) {
    if (!strchr(input, '\n')) {
        int ch;
        while ((ch = getchar()) != EOF && ch != '\n') {}
    }
}

void remove_newline_character(char *input) {
    input[strcspn(input, "\n")] = 0;
}