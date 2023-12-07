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
        printf("Erro! Não há tabelas.\n");
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

    while (table.num_columns < MAX_NUM_COLUMNS) {
        printf("Digite o nome da %dª coluna (ENTER para encerrar): ", table.num_columns + 1);

        table.columns[table.num_columns].column_name = (char *)malloc(
            sizeof(char) * MAX_DATA_LENGTH
        );
        table.columns[table.num_columns].column_name = read_data("");

        if (strcmp(table.columns[table.num_columns].column_name, "") == 0) break;

        table.columns[table.num_columns].type = choose_type();

        table.num_columns++;
    }

    table_file = fopen(table.tablename, "w");

    if (table_file == NULL) {
        printf("Erro ao criar tabela.\n");
        return false;
    }

    // Save column names
    fprintf(table_file, "%s", table.pk_name);
    for (int i = 0; i < table.num_columns; i++) {
        fprintf(table_file, ";%s", table.columns[i].column_name);
    }
    fprintf(table_file, "\n");

    // Save type of columns
    fprintf(table_file, "%d", UINTEGER);
    for (int i = 0; i < table.num_columns; i++) {
        fprintf(table_file, ";%d", table.columns[i].type);
    }
    fprintf(table_file, "\n");

    fclose(table_file);

    if (!save_table(table.tablename)) {
        printf("Erro ao criar tabela.\n");
        remove(table.tablename);
        return false;
    }

    return true;
}

bool drop_table() {
    FILE *file, *temp_file;
    char *buffer, *new_table, *table;

    printf("Digite o nome da tabela: ");
    table = read_data();

    if (!file_exists(table)) {
        printf("Erro! A tabela não existe.\n");
        return false;
    }

    new_table = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);
    buffer = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

    strcpy(new_table, "__temp");
    strcat(new_table, TABLE_LIST_FILE);

    file = fopen(TABLE_LIST_FILE, "r");
    temp_file = fopen(new_table, "w");

    if (file == NULL || temp_file == NULL) {
        printf("Erro ao apagar tabela!\n");
        return false;
    }

    while (fgets(buffer, MAX_DATA_LENGTH, file)) {
        remove_newline_character(buffer);
        
        if (strcmp(buffer, table) != 0) {
            fprintf(temp_file, "%s\n", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(TABLE_LIST_FILE);
    rename(new_table, TABLE_LIST_FILE);
    remove(table);

    free(new_table);
    free(buffer);

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

bool insert_data() {
    Table table;
    Array colum_names, colum_types;
    char *new_line = NULL;
    FILE *file;

    printf("Digite o nome da tabela: ");
    table.tablename = read_data();

    if (!file_exists(table.tablename)) {
        printf("Erro! Essa tabela não existe!\n");
        return false;
    }

    colum_names = split_string(getline(table.tablename, 1), ";");
    colum_types = split_string(getline(table.tablename, 2), ";");

    table.num_columns = colum_names.length;
    for (int i = 0; i < colum_names.length; i++) {
        table.columns[i].column_name = colum_names.values[i];
        table.columns[i].type = atoi(colum_types.values[i]);
    }

    char *user_input = NULL;
    table.data.fields = (char **)malloc(sizeof(char *) * MAX_NUM_COLUMNS);
    for (int i = 0; i < table.num_columns; i++) {
        printf("Valor do campo *%s*: ", table.columns[i].column_name);
        user_input = read_data();

        // Primary key validation
        if (i == 0 && !available_pk(table.tablename, user_input)) {
            printf("Erro! Essa chave primária já está em uso.\n");
            return false;
        }

        table.data.fields[i] = (char *)malloc(sizeof(char) * strlen(user_input));

        switch (table.columns[i].type) {
            case INTEGER:
                if (!is_int(user_input)) {
                    printf("Type error! Não é possível atribuir esse valor a uma coluna do tipo 'INTEGER'.\n");
                    return false;
                }
                strcpy(table.data.fields[i], user_input);
                break;
            case FLOAT:
                if (!is_float(user_input)) {
                    printf("Type error! Não é possível atribuir esse valor a uma coluna do tipo 'FLOAT'.\n");
                    return false;
                }
                strcpy(table.data.fields[i], user_input);
                break;
            case STRING:
                strcpy(table.data.fields[i], user_input);
                break;
            case UINTEGER:
                if (!is_uint(user_input)) {
                    printf("Type error! Não é possível atribuir esse valor a uma coluna do tipo 'UINTEGER' (Chave primária).\n");
                    return false;
                }
                strcpy(table.data.fields[i], user_input);
                break;
            default:
                strcpy(table.data.fields[i], user_input);
                break;
        }
    }

    new_line = join_string(table.data.fields, ";", table.num_columns);

    file = fopen(table.tablename, "a+");

    if (file == NULL) {
        printf("Erro ao inserir dados na tabela.\n");
        return false;
    }

    fprintf(file, "%s\n", new_line);

    fclose(file);

    free(new_line);
    free(table.data.fields);

    return true;
}

bool delete_data() {
    char *tablename = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);
    char *primary_key = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);
    FILE *file, *temp_file;
    char *temp_filename, *pk_to_delete, *text;

    printf("Digite o nome da tabela: ");
    tablename = read_data();

    if (!file_exists(tablename)) {
        printf("Erro! Essa tabela não existe.\n");
        free(tablename);
        return false;
    }

    printf("Digite o valor da chave primária: ");
    primary_key = read_data();

    if (!is_uint(primary_key)) {
        printf("Erro! Valor inválido para chave primária.\n");
        free(primary_key);
        return false;
    }

    if (available_pk(tablename, primary_key)) {
        printf(
            "Erro! Não há registro com chave primária '%s' na tabela '%s'.\n",
            primary_key,
            tablename
        );
        return false;
    }

    file = fopen(tablename, "r");
    if (file == NULL) {
        printf("Erro ao tentar apagar registro.\n");
        return false;
    }

    temp_filename = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);
    strcat(temp_filename, "__temp__");
    strcat(temp_filename, tablename);

    temp_file = fopen(temp_filename, "w");
    if (file == NULL) {
        printf("Erro ao tentar apagar registro.\n");
        free(temp_filename);
        return false;
    }

    pk_to_delete = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);
    text = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);

    int line = 1;
    while (fgets(text, MAX_LINE_LENGTH, file)) {
        if (line <= 2)
            fprintf(temp_file, "%s", text);
        else {
            sscanf(text, "%[^;]", pk_to_delete);

            if (strcmp(pk_to_delete, primary_key) != 0)
                fprintf(temp_file, "%s", text);
        };
        line++;
    }

    fclose(temp_file);
    fclose(file);

    remove(tablename);
    rename(temp_filename, tablename);

    free(tablename);
    free(primary_key);
    free(temp_filename);
    free(pk_to_delete);
    free(text);

    return true;
}