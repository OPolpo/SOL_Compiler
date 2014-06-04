#include "symbol_table.h"

int oid = 1;


Phash_node create_symbol_table(Pnode root, Phash_node * local_env){
    Pnode current, child;
    switch (root->type) {
        case T_NONTERMINAL:
            switch (root->value.ival) {
                case NPROGRAM:
                    create_symbol_table(root->child, local_env);
                    break;
                case NFUNC_DECL:
                    current = root->child; //ID
                    
                    Phash_node func = new_function_node(current->value.sval);
                    Phash_node * loc = new_hash_table();
                    func->locenv = loc;
                    int loc_oid = 1;
                    
                    func->formals_num = 0;
                    current = current->brother; //DECL_LIST_OPT
                    
                    if (current->child != NULL) {//handle parameters
                        child = current->child; //DECL
                        Formal * last_formal = func->formal;
                        while (child != NULL) { //loop on DECL
                            Pnode id_list = child->child;
                            Pschema domain_sch = create_schema(id_list->brother);
                            
                            Pnode id = id_list->child;
                            while (id != NULL){ //loop on IDs
                                Phash_node id_node = new_id_node(id->value.sval, CLPAR, loc_oid);
                                loc_oid++;
                                id_node->schema = domain_sch;
                                insert(id_node, func->locenv);
                                
                                Formal * to_add = (Formal *)malloc(sizeof(Formal));
                                to_add->formal = id_node;
                                
                                if(last_formal==NULL)
                                    func->formal=to_add;
                                else
                                    last_formal->next=to_add;
                                last_formal = to_add;
                                
                                func->formals_num++;//on single ID
                                id = id->brother;
                            };
                            child = child->brother; //next DECL
                        }
                        
                    }
                    
                    current = current->brother; //DOMAIN
                    func->schema = create_schema(current);
                    
                    current = current->brother; //TYPE_SECT_OPT
                    if (current->child != NULL) {//handle types
                        
                    }
                    
                    current = current->brother; //VAR_SECT_OPT
                    if (current->child != NULL) {//handle variables
                        
                    }
                    
                    current = current->brother; //CONST_SECT_OPT
                    if (current->child != NULL) {//handle constants
                        
                    }
                    
                    current = current->brother; //FUNC_LIST_OPT
                    if (current->child != NULL) {//handle func declaration
                        
                    }
                    
                    
                    print_func_node(func);
                    
                    
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
            
            
            break;
        case T_ID:
            
            break;
        case T_ATOMIC_DOMAIN:
            
            break;
        case T_CHARCONST:
            
            break;
        case T_INTCONST:
            
            break;
        case T_REALCONST:
            
            break;
        case T_STRCONST:
            
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

Phash_node new_function_node(char * _name){
    Phash_node node = (Phash_node) malloc (sizeof(Hash_node));
    node->name = _name;
    node->oid = oid;
    oid++;
    node->class_node = CLFUNC;
    return node;
}

Phash_node new_id_node(char * _name, Class _class, int loc_oid){
    Phash_node node = (Phash_node) malloc (sizeof(Hash_node));
    node->name = _name;
    node->oid = loc_oid;
    node->class_node = _class;
    return node;
}

Pschema create_schema(Pnode p){
    return NULL;
}
// *
//  typedef struct shash_node{
//  char * name;
//  int oid;
//  Class class_node; //TYPE, VAR, CONST, FUNC, PAR
//  Schema * schema;
//  struct shash_node ** locenv;
//  int formals_num;
//  Formal * formal;
//  struct shash_node * next;
//  } Hash_node, * Phash_node;
//  *