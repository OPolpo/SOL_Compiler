#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "hash_table.h"
#include "def.h"


Phash_node create_symbol_table(Pnode root, Phash_node * local_env);

#endif