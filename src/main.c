#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "sgbdtools.h"

int main() {
    char *cmd = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

    printf("Seja bem-vindo ao SGDBITP 1.0\n");
    printf("Digite \"help\" para mais informações.\n");
    while (true) {
        printf("sgbditp> ");
        if (fgets(cmd, MAX_DATA_LENGTH, stdin) == NULL) break;

        clear_buffer(cmd);
        remove_newline_character(cmd);
        
        if (strcmp(cmd, "help") == 0) {
            show_help_message();
        } else if (strcmp(cmd, "exit") == 0) {
            break;
        } else if (strcmp(cmd, "clear") == 0) {
            clear_screen();
        } else if (strcmp(cmd, "create") == 0) {
            create_table();
        } else if (strcmp(cmd, "show") == 0) {
            show_tables();
        } else if (strcmp(cmd, "drop") == 0) {
            drop_table();
        } else if (strcmp(cmd, "insert") == 0) {
            insert_data();
        } else if (strcmp(cmd, "delete") == 0) {
            delete_data();
        } else {
            printf("Esse comando não existe.\n");
        }
    }
    
    free(cmd);

    return 0;
}