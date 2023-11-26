#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main()
{
    char *tablename;

    printf("Enter the table name: ");
    tablename = input();

    printf("Table name: %s\n", tablename);

    free(tablename);

    return 0;
}