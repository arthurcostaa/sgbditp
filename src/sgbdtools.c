#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

#define MAX_TABLE_NAME_LENGTH 65
#define MAX_COLUMN_NAME_LENGTH 65
#define MAX_DATA_LENGTH 257

const char *TABLE_LIST = "__table_list__";

void store_table(char *tablename) {
    FILE *table_list_file;

    table_list_file = fopen(TABLE_LIST, "a+");

    fprintf(table_list_file, "%s\n", tablename);

    fclose(table_list_file);
}

void show_tables() {
    FILE *table_list = fopen(TABLE_LIST, "r");
    char file_line[MAX_TABLE_NAME_LENGTH];

    if (table_list == NULL) {
        printf("Erro ao ler arquivo com nomes das tabelas.\n");
        printf("Provavelmente não foi criado nenhuma tabela.\n");
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
    clear_buffer(tablename);
    remove_newline_character(tablename);

    if ((int)strlen(tablename) == 0) {
        printf("Erro! É necessário dar um nome para a sua tabela!\n");
        return;
    }

    if (file_exists(tablename)) {
        printf("Erro! Já existe uma tabela com esse nome!\n");
        return;
    }

    while (true) {
        // The first column is the primary key
        if (column_number == 0)
            printf("Digite o nome da %dª coluna (PRIMARY KEY): ", column_number + 1);
        else
            printf("Digite o nome da %dª coluna: ", column_number + 1);

        fgets(column_name, MAX_COLUMN_NAME_LENGTH, stdin);
        clear_buffer(column_name);
        remove_newline_character(column_name);

        if ((int)strlen(column_name) == 0 && column_number == 0) {
            printf("Erro! A sua tabela deve ter pelo menos uma coluna!\n");
            return;
        }

        // Stop entering column names
        if ((int)strlen(column_name) == 0)
            break;

        columns[column_number] = (char *)malloc(strlen(column_name));
        strcpy(columns[column_number], column_name);

        column_number++;

        columns = (char **)realloc(columns, (column_number + 1) * sizeof(char *));
    }

    table = fopen(tablename, "w");

    for (int i = 0; i < column_number; i++) {
        if (i < column_number - 1)
            fprintf(table, "%s;", columns[i]);
        else
            fprintf(table, "%s\n", columns[i]);
    }

    store_table(tablename);
    fclose(table);
    free(columns);
}

void drop_table() {
    char tablename[MAX_TABLE_NAME_LENGTH];
    char temp_filename[MAX_TABLE_NAME_LENGTH];
    FILE *file, *temp_file;
    char buffer[MAX_TABLE_NAME_LENGTH];

    printf("Cuidado! Essa é uma operação permanente e todos os seus dados serão perdidos.\n");
    printf("Digite o nome da tabela que será apagada: ");
    fgets(tablename, MAX_TABLE_NAME_LENGTH, stdin);
    clear_buffer(tablename);
    remove_newline_character(tablename);

    if ((int)strlen(tablename) == 0) {
        printf("Erro! Não foi informado o nome da tabela!\n");
        return;
    }

    if (!file_exists(tablename)) {
        printf("Erro! Essa tabela não existe!\n");
        return;
    }

    strcpy(temp_filename, "__temp");
    strcat(temp_filename, TABLE_LIST);

    file = fopen(TABLE_LIST, "r");
    temp_file = fopen(temp_filename, "w");

    while (fgets(buffer, MAX_TABLE_NAME_LENGTH, file)) {
        remove_newline_character(buffer);
        if (strcmp(buffer, tablename) != 0) {
            fprintf(temp_file, "%s\n", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(TABLE_LIST);
    rename(temp_filename, TABLE_LIST);
    remove(tablename);
}

void insert_data() {
    char tablename[MAX_TABLE_NAME_LENGTH];
    char *columns_line = NULL;
    char **data_array = NULL;
    char data[MAX_DATA_LENGTH];
    char *separator = ";";
    char *new_line = NULL;
    Array column_names;
    FILE *table;

    printf("Digite o nome da tabela: ");
    fgets(tablename, MAX_TABLE_NAME_LENGTH, stdin);
    clear_buffer(tablename);
    remove_newline_character(tablename);

    if (!file_exists(tablename)) {
        printf("Erro! Essa tabela não existe!\n");
        return;
    }

    columns_line = readline(tablename);
    column_names = split_string(columns_line, ";");

    data_array = (char **)malloc(sizeof(char *) * column_names.length);

    for (int i = 0; i < column_names.length; i++) {
        printf("Valor de *%s*: ", column_names.values[i]);
        fgets(data, MAX_DATA_LENGTH, stdin);
        clear_buffer(data);
        remove_newline_character(data);

        data_array[i] = (char *)malloc(sizeof(char) * strlen(data));
        strcpy(data_array[i], data);
    }

    new_line = join_string(data_array, separator, column_names.length);

    table = fopen(tablename, "a+");
    fprintf(table, "%s\n", new_line);
    fclose(table);

    free(data_array);
    free(columns_line);
    free(new_line);

    // TODO: Validate data before insert
}