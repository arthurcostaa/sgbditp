#include "sgbdtools.h"

#ifndef UTILS
#define UTILS

typedef struct {
    int length;
    char **values;
} Array;

#endif


char *input();
bool file_exists(char *filename);
void clear_buffer(char *input);
void remove_newline_character(char *input);
char *readline(char *filename);
Array split_string(char *text, char *separator);
char *join_string(char **array_text, char *separator, int num_items);
char *read_data();
Types choose_type();
char *getline(char *filename, int line);
bool is_int(char *number);
bool is_float(char *number);
bool is_uint(char *number);
void show_help_message();