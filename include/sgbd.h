#ifndef SGBD
#define SGBD

#define MAX_DATA_LENGTH 65
#define MAX_NUM_COLUMNS 20
#define MAX_LINE_LENGTH (MAX_DATA_LENGTH * MAX_NUM_COLUMNS + MAX_NUM_COLUMNS - 1)
#define TABLE_LIST_FILE "__table_list__"

typedef enum {
    INTEGER = 0,
    FLOAT = 1,
    STRING = 2,
    UINTEGER = 3
} Types;

typedef struct {
    char **fields;
} Tuple;

typedef struct {
    char *column_name;
    Types type;
} Column;

typedef struct {
    char *tablename;
    Column columns[MAX_NUM_COLUMNS];
    int num_columns;
    unsigned int primary_key;
    char *pk_name;
    Tuple data;
} Table;

#endif
