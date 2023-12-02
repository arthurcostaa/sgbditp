#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sgbdtools.h"
#include "utils.h"

const char *TABLE_LIST_FILE = "__table_list__";

bool save_table(char *tablename) {
    FILE *file;

    file = fopen(TABLE_LIST_FILE, "a+");

    if (file == NULL)
        return false;

    fprintf(file, "%s\n", tablename);

    fclose(file);

    return true;
}

bool show_tables() {
    FILE *file = fopen(TABLE_LIST_FILE, "r");
    char file_line[MAX_DATA_LENGTH];

    if (file == NULL) {
        printf("Erro! Não há um registro de tabelas.\n");
        return false;
    }

    while (fgets(file_line, MAX_DATA_LENGTH, file))
        printf("%s", file_line);

    fclose(file);

    return true;
}

bool create_table() {
    Table table;
    table.num_columns = 0;
    FILE *table_file;

    printf("Digite o nome da tabela: ");
    table.tablename = read_data();

    if ((int)strlen(table.tablename) == 0) {
        printf("Erro! É necessário dar um nome para a sua tabela!\n");
        return false;
    }

    if (file_exists(table.tablename)) {
        printf("Erro! Já existe uma tabela com esse nome!\n");
        return false;
    }

    printf("Digite o nome da coluna chave primária: ");
    table.pk_name = read_data();

    while (true) {
        printf("Digite o nome da %dª coluna: ", table.num_columns + 1);

        table.columns = (Column *)realloc(
            table.columns,
            sizeof(Column) * (table.num_columns + 1)
        );
        table.columns[table.num_columns].column_name = (char *)malloc(
            sizeof(char) * MAX_DATA_LENGTH
        );
        table.columns[table.num_columns].column_name = read_data("");

        if (strcmp(table.columns[table.num_columns].column_name, "") == 0) break;

        table.columns[table.num_columns].type = choose_type();

        printf("Type: %d\n", table.columns[table.num_columns].type);

        table.num_columns++;
    }

    table_file = fopen(table.tablename, "w");

    if (table_file == NULL) {
        printf("Erro ao criar tabela.\n");
        return false;
    }

    fprintf(table_file, "%s", table.pk_name);
    for (int i = 0; i < table.num_columns; i++) {
        fprintf(table_file, ";%s", table.columns[i].column_name);
    }
    fprintf(table_file, "\n");

    fprintf(table_file, "%d", INTEGER);
    for (int i = 1; i < table.num_columns; i++) {
        fprintf(table_file, ";%d", table.columns[i].type);
    }
    fprintf(table_file, "\n");

    fclose(table_file);
    free(table.columns);

    if (!save_table(table.tablename)) {
        printf("Erro ao criar tabela.\n");
        remove(table.tablename);
        return false;
    }

    return true;
}

// void drop_table() {
//     char tablename[MAX_DATA_LENGTH];
//     char temp_filename[MAX_DATA_LENGTH];
//     FILE *file, *temp_file;
//     char buffer[MAX_DATA_LENGTH];

//     printf("Cuidado! Essa é uma operação permanente e todos os seus dados serão perdidos.\n");
//     printf("Digite o nome da tabela que será apagada: ");
//     fgets(tablename, MAX_DATA_LENGTH, stdin);
//     clear_buffer(tablename);
//     remove_newline_character(tablename);

//     if ((int)strlen(tablename) == 0) {
//         printf("Erro! Não foi informado o nome da tabela!\n");
//         return;
//     }

//     if (!file_exists(tablename)) {
//         printf("Erro! Essa tabela não existe!\n");
//         return;
//     }

//     strcpy(temp_filename, "__temp");
//     strcat(temp_filename, TABLE_LIST);

//     file = fopen(TABLE_LIST, "r");
//     temp_file = fopen(temp_filename, "w");

//     while (fgets(buffer, MAX_DATA_LENGTH, file)) {
//         remove_newline_character(buffer);
//         if (strcmp(buffer, tablename) != 0) {
//             fprintf(temp_file, "%s\n", buffer);
//         }
//     }

//     fclose(file);
//     fclose(temp_file);

//     remove(TABLE_LIST);
//     rename(temp_filename, TABLE_LIST);
//     remove(tablename);
// }

// void insert_data() {
//     char tablename[MAX_DATA_LENGTH];
//     char *columns_line = NULL;
//     char **data_array = NULL;
//     char data[MAX_DATA_LENGTH];
//     char *separator = ";";
//     char *new_line = NULL;
//     Array column_names;
//     FILE *table;

//     printf("Digite o nome da tabela: ");
//     fgets(tablename, MAX_DATA_LENGTH, stdin);
//     clear_buffer(tablename);
//     remove_newline_character(tablename);

//     if (!file_exists(tablename)) {
//         printf("Erro! Essa tabela não existe!\n");
//         return;
//     }

//     columns_line = readline(tablename);
//     column_names = split_string(columns_line, ";");

//     data_array = (char **)malloc(sizeof(char *) * column_names.length);

//     for (int i = 0; i < column_names.length; i++) {
//         printf("Valor de *%s*: ", column_names.values[i]);
//         fgets(data, MAX_DATA_LENGTH, stdin);
//         clear_buffer(data);
//         remove_newline_character(data);

//         data_array[i] = (char *)malloc(sizeof(char) * strlen(data));
//         strcpy(data_array[i], data);
//     }

//     new_line = join_string(data_array, separator, column_names.length);

//     table = fopen(tablename, "a+");
//     fprintf(table, "%s\n", new_line);
//     fclose(table);

//     free(data_array);
//     free(columns_line);
//     free(new_line);

//     // TODO: Validate data before insert
// }