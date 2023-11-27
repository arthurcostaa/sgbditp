#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

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

    fclose(table);
    free(tablename);
}