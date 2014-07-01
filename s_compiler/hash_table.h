#ifndef HASH_TABLE
#define HASH_TABLE

#define TOT 313
#define SHIFT 4
#include <string.h>
#include "def.h"
#include "../s_shared/schema.h"

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

typedef struct saux_fun_param{
    int last_oid;
    int num_obj;
    struct shash_node ** locenv;
    int formals_num;
    Formal * formal;
} Aux_fun_param, *Paux_fun_param;

typedef struct shash_node{
    char * name;
    int oid; 
    Class class_node; //TYPE, VAR, CONST, FUNC, PAR
    Pschema schema;
    Paux_fun_param aux;
    struct shash_node * father;
    struct shash_node * next;
} Hash_node, * Phash_node;

int hash (char* id);
int insert(Phash_node p,Phash_node * table);
int insert_func(Phash_node p, Phash_node f_parent);
Phash_node getNode(char * id, Phash_node * table);
Phash_node find_visible_node(char * id, Phash_node func_node, int * offset);

Phash_node * new_hash_table();

void print_func_node(Phash_node node);
void print_generic_node(Phash_node node);
void print_hash_content(Phash_node * table);

#endif