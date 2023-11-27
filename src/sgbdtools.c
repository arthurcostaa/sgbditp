#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

const char *TABLE_LIST = "__table_list__";

void store_table(char *tablename)
{
    FILE *table_list_file;

    table_list_file = fopen(TABLE_LIST, "a+");

    fputs(tablename, table_list_file);

    fputc('\n', table_list_file);

    fclose(table_list_file);
    
}

void show_tables()
{
    FILE *table_list = fopen(TABLE_LIST, "r");

    if (table_list == NULL)
    {
        printf("Erro ao ler arquivo com nomes das tabelas.\n");
        return;
    }
}

void create_table()
{
    char *tablename;
    char *column_name;
    int column_number = 1;
    FILE *table;

    printf("Digite o nome da tabela: ");
    tablename = input();

    if (file_exists(tablename))
    {
        printf("Erro! Essa tabela já existe!\n");
        return;
    }

    if ((int)strlen(tablename) == 0)
    {
        printf("Erro! Não foi informado o nome da tabela!\n");
        return;
    }

    table = fopen(tablename, "w");

    while (true)
    {
        printf("Digite o nome da %dª coluna: ", column_number);
        column_name = input();

        if (column_number == 1 && (int)strlen(column_name) == 0)
        {
            printf("Erro! A sua tabela deve ter pelo menos uma coluna!\n");
            remove(tablename);
            return;
        }

        if ((int)strlen(column_name) == 0)
            break;

        column_number > 1
            ? fprintf(table, ";%s", column_name)
            : fprintf(table, "%s", column_name);

        column_number++;
    }

    fputc('\n', table);

    store_table(tablename);
    fclose(table);
    free(tablename);
}