#include "format_parser.h"
#include "../s_shared/schema.h"

typedef enum{
    F_VECTOR,
    F_STRUCT,
    F_CHARCONST,
    F_INTCONST,
    F_REALCONST,
    F_STRCONST,
    F_BOOLCONST,
    F_TEMP
}Typeformatted;

typedef struct sformatted{
    Typeformatted type;
    Value value;
    char * id;
    struct sformatted *child, *brother;
} Formatted, * Pformatted;

typedef enum{
    T_CHAR,
    T_INT, 
    T_REAL,
    T_STRING,
    T_BOOL,
    T_STRUCT,
    T_VECTOR,
    T_ATTR
} Type;

typedef struct sformat{
    Type type;
    char *id;
    int size;
    struct sformat *child;
    struct sformat *brother;
}Format, *Pformat;

void print_atomic_istack(FILE* stream, char * elem_addr, Pschema elem_type);

void print_vector(FILE* stream, char * elem_addr, int elem_num, Pschema elem_type);

void print_struct(FILE* stream, char * elem_addr, Pschema elem_type);

void basic_wr(FILE* stream, char* format);