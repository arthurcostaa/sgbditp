#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "sgbdtools.h"

int main() {

    create_table();

    printf("\nTabelas:\n");

    show_tables();

    return 0;
}