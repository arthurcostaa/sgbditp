#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>

#include "sgbdtools.h"
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

char *read_data() {
    char *data = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

    fgets(data, MAX_DATA_LENGTH, stdin);
    clear_buffer(data);
    remove_newline_character(data);

    return data;
}

Types choose_type() {
    int choice;

    printf("Escolha o tipo da coluna:\n");
    printf("1 - INT\n");
    printf("2 - FLOAT\n");
    printf("3 - STR\n");

    printf("Digite o número correspondente ao tipo desejado: ");
    scanf("%d", &choice);
    getchar(); // clear buffer

    switch (choice) {
        case 1:
            return INTEGER;
        case 2:
            return FLOAT;
        case 3:
            return STRING;
        default:
            printf("Erro! Escolha inválida. Valor padrão será STRING.\n");
            return STRING;
    }
}

char *getline(char *filename, int line) {
    FILE *file;
    char *string = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    int num_lines = 0;

    file = fopen(filename, "r");

    while (fgets(string, sizeof(char) * MAX_LINE_LENGTH, file) != NULL) {
        num_lines++;

        if (num_lines == line) break;
    }

    fclose(file);

    remove_newline_character(string);

    return string;
}

bool is_int(char *number) {
    char *reminder;
    int base = 10;

    strtol(number, &reminder, base);

    return *reminder == '\0';
}

bool is_uint(char *number) {
    char *reminder;
    int base = 10;

    long int result = strtol(number, &reminder, base);

    return *reminder == '\0' && result > 0;
}

bool is_float(char *number) {
    char *reminder;

    strtod(number, &reminder);

    return *reminder == '\0';
}

void show_help_message() {
    printf("Comandos disponíveis:\n");
    printf("  help - mostra essa mensagem de ajuda\n");
    printf("  clear - limpa a tela\n");
    printf("  create - cria uma nova tabela\n");
    printf("  drop - apaga uma tabela\n");
    printf("  select - lista todos os dados de uma tabela\n");
    printf("  search - pesquisar dados em uma tabela\n");
    printf("  delete - apagar uma tabela\n");
    printf("  show - mostrar todas a tabelas criadas\n");
    printf("  exit - sair do repl\n");
    printf("\nDigite Ctrl-D para sair do repl\n");
}