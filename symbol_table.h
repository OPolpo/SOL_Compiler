#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "hash_table.h"
#include "def.h"


Phash_node create_symbol_table(Pnode root);
Phash_node new_function_node(char * _name);
Phash_node new_id_node(char * _name, Class _class, int loc_id);
Pschema new_schema_node(int _type);

Pschema create_schema(Pnode root);
Pschema create_domain_schema(Pnode domain, char * id);
#endif