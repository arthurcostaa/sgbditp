#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>

#include "utils.h"

#define INITIAL_SIZE 10

char *input() {
    char *str = (char *)malloc(sizeof(char) * INITIAL_SIZE);
    char c;

    size_t size = 0;

    while (true) {
        c = getchar();

        if (c == '\n')
            break;

        if (size > INITIAL_SIZE) {
            str = (char *)realloc(str, sizeof(char) * (size + INITIAL_SIZE));
        }

        str[size++] = c;
    }

    str = (char *)realloc(str, sizeof(char) * (size + 1));
    str[size] = '\0';

    return str;
}

bool file_exists(char *filename) {
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

char *readline(char *filename) {
    int c;
    size_t length = 0;
    char *line = NULL;
    FILE *file;

    file = fopen(filename, "r");

    while ((c = fgetc(file)) != EOF && c != '\n') {
        line = (char *)realloc(line, sizeof(char) * (length + 1));
        line[length++] = c;
    }

    line = (char *)realloc(line, sizeof(char) * (length + 1));
    line[length] = '\0';

    fclose(file);

    return line;
}

Array split_string(char *text, char *separator) {
    char **strings_array = NULL;
    size_t array_length = 0;
    char *piece = NULL;
    Array array;

    piece = strtok(text, separator);

    while (piece != NULL) {
        strings_array = (char **)realloc(
            strings_array,
            sizeof(char *) * (array_length + 1)
        );
        strings_array[array_length] = (char *)malloc(
            sizeof(char) * strlen(piece)
        );
        strcpy(strings_array[array_length], piece);

        array_length++;
        piece = strtok(NULL, separator);
    }

    array.length = array_length;
    array.values = strings_array;

    return array;
}

char *join_string(char **array_text, char *separator, int num_items) {
    char *string = (char *)malloc(sizeof(char));
    string[0] = '\0';
    size_t size_str = 0;

    for (int i = 0; i < num_items; i++) {
        size_str += strlen(array_text[i]) + strlen(separator);
        printf("Size of size_str: %ld\n", size_str);
        string = (char *)realloc(string, sizeof(char) * (size_str + 1));

        if (i < num_items - 1) {
            strcat(string, array_text[i]);
            strcat(string, separator);
        } else {
            strcat(string, array_text[i]);
        }
    }

    return string;
}