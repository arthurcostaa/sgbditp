#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "sgbd.h"

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


bool save_table(char *tablename) {
    FILE *file;

    file = fopen(TABLE_LIST_FILE, "a+");

    if (file == NULL) {
        printf("Erro ao criar tabela.\n");
        return false;
    }

    fprintf(file, "%s\n", tablename);

    fclose(file);

    return true;
}


bool available_pk(char *table, char *pk) {
    FILE *file;
    int line_number = 1;
    char *text = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    Array value;

    file = fopen(table, "r");

    if (file == NULL) {
        printf("Erro ao ler verificar chave primária.\n");
        return false;
    }

    while (fgets(text, MAX_LINE_LENGTH, file)) {
        if (!(line_number <= 2)) {
            value = split_string(text, ";");

            if (strcmp(pk, value.values[0]) == 0) return false;
        }
        line_number++;
    }

    fclose(file);
    free(text);

    return true;
}


Array *separate_table_values(char *tablename, int *num_rows) {
    Array *table_data = NULL, row_data;
    int line_number = 1;
    char *row;
    FILE *file;

    file = fopen(tablename, "r");

    row = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);

    while (fgets(row, MAX_LINE_LENGTH, file)) {
        table_data = (Array *)realloc(
            table_data,
            sizeof(Array) * line_number
        );

        remove_newline_character(row);
        row_data = split_string(row, ";");
        table_data[line_number - 1] = row_data;

        line_number++;
    }

    *num_rows = line_number - 1;

    return table_data;
}


int *max_column_lengths(Array *data, int num_cols, int num_rows) {
    int *column_lengths = (int *)malloc(sizeof(int) * num_cols);

    memset(column_lengths, 0, num_cols);

    for (int row = 0; row < num_rows; row++) {
        if (row == 1) continue;

        for (int col = 0; col < num_cols; col++) {
            if ((int)strlen(data[row].values[col]) > column_lengths[col]) {
                column_lengths[col] = (int)strlen(data[row].values[col]);
            }
        }
    }

    return column_lengths;
}


void print_table(Array *table_data,
                 int *column_lengths,
                 int num_rows,
                 int num_cols) {
    for (int row = 0; row < num_rows; row++) {
        if (row == 1) continue;

        for (int col = 0; col < num_cols; col++)
            printf(" %-*s |", column_lengths[col], table_data[row].values[col]);

        printf("\n");
    }
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


void print_search_options() {
    printf("\nOpções de pesquisa:\n");
    printf("1. Valores maior que o valor informado\n");
    printf("2. Valores maior ou igual que o valor informado\n");
    printf("3. Valores igual o valor informado\n");
    printf("4. Valores menor que o valor informado\n");
    printf("5. valores menor ou igual que o valor informado\n");
    printf("6. Valores próximo ao valor informado (apenas tipo STRING)\n");
}