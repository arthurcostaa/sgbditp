#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

#define MAX_TABLE_NAME_LENGTH 65
#define MAX_COLUMN_NAME_LENGTH 65

const char *TABLE_LIST = "__table_list__";

void store_table(char *tablename) {
    FILE *table_list_file;

    table_list_file = fopen(TABLE_LIST, "a+");

    fputs(tablename, table_list_file);

    fputc('\n', table_list_file);

    fclose(table_list_file);
}

void show_tables() {
    FILE *table_list = fopen(TABLE_LIST, "r");
    char file_line[MAX_TABLE_NAME_LENGTH];

    if (table_list == NULL) {
        printf("Erro ao ler arquivo com nomes das tabelas.\n");
        return;
    }

    while (fgets(file_line, MAX_TABLE_NAME_LENGTH, table_list))
        printf("%s", file_line);

    fclose(table_list);
}

void create_table() {
    char tablename[MAX_COLUMN_NAME_LENGTH];
    char column_name[MAX_COLUMN_NAME_LENGTH];
    int column_number = 0;
    char **columns = (char **)malloc((column_number + 1) * sizeof(char *));
    FILE *table;

    printf("Digite o nome da tabela: ");
    fgets(tablename, MAX_TABLE_NAME_LENGTH, stdin);

    // Clear buffer
    if (!strchr(tablename, '\n')) {
        int ch;
        while ((ch = getchar()) != EOF && ch != '\n') {}
    }

    tablename[strcspn(tablename, "\n")] = 0;

    if ((int)strlen(tablename) == 0) {
        printf("Erro! É necessário dar um nome para a sua tabela!\n");
        return;
    }

    if (file_exists(tablename)) {
        printf("Erro! Já existe uma tabela com esse nome!\n");
        return;
    }

    // Read table columns
    while (true) {
        printf("Digite o nome da %dª coluna: ", column_number + 1);
        fgets(column_name, MAX_COLUMN_NAME_LENGTH, stdin);

        // Clear buffer
        if (!strchr(column_name, '\n')) {
            int ch;
            while (((ch = getchar()) != EOF) && (ch != '\n')) {}
        }

         // Remove '\n' from string
        column_name[strcspn(column_name, "\n")] = 0;

        if ((int)strlen(column_name) == 0 && column_number == 0) {
            printf("Erro! A sua tabela deve ter pelo menos uma coluna!\n");
            return;
        }

        // Stop entering column names
        if ((int)strlen(column_name) == 0)
            break;


        columns[column_number] = (char *)malloc(strlen(column_name)); // adicionar mais 1?
        strcpy(columns[column_number], column_name);

        column_number++;

        columns = (char **)realloc(columns, (column_number + 1) * sizeof(char *));
    }

    table = fopen(tablename, "w");

    for (int i = 0; i < column_number; i++) {
        if (i < column_number - 1)
            fprintf(table, "%s;", columns[i]);
        else
            fprintf(table, "%s", columns[i]);
    }


    store_table(tablename);
    fclose(table);
    free(columns);
}