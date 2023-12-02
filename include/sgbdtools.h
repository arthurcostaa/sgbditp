#ifndef SGBDTOOLS
#define SGBDTOOLS

#define MAX_DATA_LENGTH 65

typedef enum {
    INTEGER = 0,
    FLOAT = 1,
    STRING = 2
} Types;

typedef struct {
    Types type;
    union {
        char _null;
        char _str[MAX_DATA_LENGTH];
        int _int;
        float _float;
    } Value;
} Field;

typedef struct {
    Field *fields;
} Tuple;

typedef struct {
    char *column_name;
    Types type;
} Column;

typedef struct {
    char *tablename;
    Column *columns;
    int num_columns;
    unsigned int primary_key;
    char *pk_name;
    Tuple *data;
} Table;

#endif

bool create_table();
bool save_table(char *tablename);
bool show_tables();
bool drop_table(char *table);
// void show_tables();
// void insert_data();