#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "hash_table.h"
#include "def.h"

Phash_node create_symbol_table(Pnode root, Phash_node * local_env);
Phash_node new_function_node(char * _name);
Phash_node new_id_node(char * _name, Class _class);
Pschema create_schema(Pnode p);

#endif