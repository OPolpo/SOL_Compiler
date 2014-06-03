#include "symbol_table.h"

int oid = 1;



Phash_node create_symbol_table(Pnode root, Phash_node * local_env){
    switch (root->type) {
        case T_NONTERMINAL: {
            switch (root->value.ival) {
                case NPROGRAM:
                    create_symbol_table(root->child, local_env);
                    break;
                case NFUNC_DECL:
                    new_function_node()
                    
                    create_symbol_table(, );
                    break;
                case NDECL_LIST_OPT:
                    
                    break;
                case NDECL:
                    
                    break;
                case NID_LIST:
                    
                    break;
                case NDOMAIN:
                    
                    break;
                case NSTRUCT_DOMAIN:
                    
                    break;
                case NVECTOR_DOMAIN:
                    
                    break;
                case NTYPE_SECT_OPT:
                    
                    break;
                case NVAR_SECT_OPT:
                    
                    break;
                case NCONST_SECT_OPT:
                    
                    break;
                case NFUNC_LIST_OPT:
                    
                    break;
                    
                default:
                    break;
            }
        }
            
            break;
        case T_ID:
            
            break;
        case T_ATOMIC_DOMAIN:
            <#statements#>
            break;
        case T_CHARCONST:
            <#statements#>
            break;
        case T_INTCONST:
            <#statements#>
            break;
        case T_REALCONST:
            <#statements#>
            break;
        case T_STRCONST:
            <#statements#>
            break;
        case T_BOOLCONST:
            
            break;
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