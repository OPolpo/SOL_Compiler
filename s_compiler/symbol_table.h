#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "hash_table.h"
#include "def.h"
#include <math.h>


Phash_node create_symbol_table(Pnode root, Phash_node father);
Phash_node new_function_node(char * _name);
Phash_node new_id_node(char * _name, Class _class, int loc_id);



Pschema new_schema_node(int _type);
int are_compatible(Pschema a, Pschema b);
Pschema create_schema(Pnode domain, Phash_node func, char * id);
int compute_size(Pschema schema);
char * schema2format(Pschema schema);
char * make_format(Pschema schema);

#endif