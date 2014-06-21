#ifndef HASH_TABLE
#define HASH_TABLE

#define TOT 313
#define SHIFT 4
#include <string.h>
#include "def.h"

typedef enum{
    CLTYPE,
    CLVAR,
    CLCONST,
    CLFUNC,
    CLPAR,
    CLCOUNT
} Class;


typedef struct sformalsnode{
    struct shash_node * formal;
    struct sformalsnode * next;
} Formal;

typedef struct sschema{
    int type; //CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR (ATTR no more)
    char * id; //field name
    int size; //array size (type = VECTOR)
    struct sschema *p1, *p2;
} Schema, * Pschema;

typedef struct shash_node{
    char * name;
    int oid;
    int last_oid;
    Class class_node; //TYPE, VAR, CONST, FUNC, PAR
    Pschema schema;
    struct shash_node ** locenv;
        int formals_num;
    Formal * formal;
    struct shash_node * father;
    struct shash_node * next;
} Hash_node, * Phash_node;

int hash (char* id);
int insert(Phash_node p,Phash_node * table);
Phash_node getNode(char * id, Phash_node * table);
Phash_node find_visible_node(char * id, Phash_node func_node, int * offset);

Phash_node * new_hash_table();
void print_func_node(Phash_node node);

void print_generic_node(Phash_node node);

void printSchema(Pschema root, char* father_indent);
void print_sch(Pschema root);
void print_hash_content(Phash_node * table);

#endif