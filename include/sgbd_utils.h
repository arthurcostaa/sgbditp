#include "utils.h"
#include "sgbd.h"
#include "stdbool.h"

typedef enum {
    GT = 1,
    GTE = 2,
    EQ = 3,
    LT = 4,
    LTE = 5,
    CLOSE = 6
} SearchOptions;

Types choose_type();

bool save_table(char *tablename);

bool available_pk(char *table, char *pk);

Array *separate_table_values(char *tablename, int *num_rows);

int *max_column_lengths(Array *data, int num_cols, int num_rows);

void print_table(
    Array *table_data,
    int *column_lengths,
    int num_rows,
    int num_cols
);

bool is_int(char *number);

bool is_uint(char *number);

bool is_float(char *number);

void show_help_message();

void print_search_options();

bool is_valid_search_option(int option);

int str_to_type_column(char *type);