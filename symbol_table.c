#include "symbol_table.h"

int oid = 1;



Phash_node create_symbol_table(Pnode root, Phash_node ){
    switch (root->type) {
        case T_NONTERMINAL:
            <#statements#>
            break;
        case 	T_ID,
            T_ATOMIC_DOMAIN,
            T_CHARCONST,
            T_INTCONST,
            T_REALCONST,
            T_STRCONST,
            T_BOOLCONST,
        default:
            break;
    }
    
    /*
    if (root == NULL) {
        return Phash_node;
    }
     */
}

Phash_node new_function_node(char * _name, Pschema _schema, int _num, Formal _formal){
    Phash_node node = (Phash_node) malloc (sizeof(Hash_node));
    node->name = _name;
    node->oid = oid;
    oid++;
    node->class_node = CLFUNC;
    node->schema = _schema;
    node->formals_num = _num;
    node->formal = _formal;
    node->locenv = new_hash_table();
}

/**
 typedef struct shash_node{
 char * name;
 int oid;
 Class class_node; //TYPE, VAR, CONST, FUNC, PAR
 Schema * schema;
 struct shash_node ** locenv;
 int formals_num;
 Formal * formal;
 struct shash_node * next;
 } Hash_node, * Phash_node;
**/