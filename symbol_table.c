#include "symbol_table.h"
#include "parser.h"

int oid = 1;


Phash_node create_symbol_table(Pnode root){
    Pnode current, child;
    switch (root->type) {
        case T_NONTERMINAL:
            switch (root->value.ival) {
                case NPROGRAM:
                    create_symbol_table(root->child);
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
                            }
                            child = child->brother; //next DECL
                        }
                        
                    }
                    
                    current = current->brother; //DOMAIN
                    func->schema = create_schema(current);
                    
                    // current = current->brother; //TYPE_SECT_OPT
                    // if (current->child != NULL) {//handle types
                    //     child = current->child; //DECL
                    //     while (child != NULL) { //loop on DECL
                    //         Pnode id_list = child->child;
                    //         Pschema domain_sch = create_schema(id_list->brother);
                            
                    //         Pnode id = id_list->child;
                    //         while (id != NULL){ //loop on IDs
                    //             Phash_node id_node = new_id_node(id->value.sval, CLTYPE, loc_oid);
                    //             loc_oid++;
                    //             id_node->schema = domain_sch;
                    //             insert(id_node, func->locenv);
                    //             id = id->brother;
                    //         }
                    //         child = child->brother; //next DECL
                    //     }
                    // }
                    
                    // current = current->brother; //VAR_SECT_OPT
                    // if (current->child != NULL) {//handle variables
                    //     child = current->child; //DECL
                    //     while (child != NULL) { //loop on DECL
                    //         Pnode id_list = child->child;
                    //         Pschema domain_sch = create_schema(id_list->brother);
                            
                    //         Pnode id = id_list->child;
                    //         while (id != NULL){ //loop on IDs
                    //             Phash_node id_node = new_id_node(id->value.sval, CLVAR, loc_oid);
                    //             loc_oid++;
                    //             id_node->schema = domain_sch;
                    //             insert(id_node, func->locenv);
                    //             id = id->brother;
                    //         }
                    //         child = child->brother; //next DECL
                    //     }
                    // }
                    
                    // current = current->brother; //CONST_SECT_OPT
                    // if (current->child != NULL) {//handle constants
                    //     child = current->child; //DECL
                    //     while (child != NULL) { //loop on DECL
                    //         Pnode id_list = child->child;
                    //         Pschema domain_sch = create_schema(id_list->brother);
                            
                    //         Pnode id = id_list->child;
                    //         while (id != NULL){ //loop on IDs
                    //             Phash_node id_node = new_id_node(id->value.sval, CLCONST, loc_oid);
                    //             loc_oid++;
                    //             id_node->schema = domain_sch;
                    //             insert(id_node, func->locenv);
                    //             id = id->brother;
                    //         }
                    //         child = child->brother->brother; //next DECL
                    //     }
                    // }
                    
                    // current = current->brother; //FUNC_LIST_OPT
                    // if (current->child != NULL) {
                    //     child = current->child; // FUNC DECL
                    //     while (child != NULL) {//loop on FUNC DECL
                    //         create_symbol_table(current);
                    //         child = child->brother;
                    //     }
                    // }
                    
                    
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

Pschema create_schema(Pnode root){// called on DOMAIN node
    return create_domain_schema(root, NULL);
}

Pschema create_domain_schema(Pnode domain, char * id){
    Pnode dom_child = domain->child;
    Pschema node;
    switch (dom_child->type) {
        case T_NONTERMINAL:
            switch (dom_child->value.ival) {
                case NSTRUCT_DOMAIN:
                    node = new_schema_node(STRUCT);
                    Pnode decl = dom_child->child;
                    
                    Pschema last = node->p1;
                    while (decl != NULL) { //DECL
                        Pnode decl_domain = decl->child->brother;
                        Pnode id = decl->child->child;
                        
                        while (id != NULL) {
                            if(last == NULL)
                                node->p1 = create_domain_schema(decl_domain, id->value.sval);
                            else
                                last->p2 = create_domain_schema(decl_domain, id->value.sval);
                            last = last->p2;
                            id = id->brother;
                        }
                        decl = decl->brother;
                    }
                    break;
                case NVECTOR_DOMAIN:
                    node = new_schema_node(VECTOR);
                    node->id = id;
                    node->size = dom_child->child->value.ival;
                    node->p1 = create_domain_schema(dom_child->child->brother, NULL);
                    break;
                default:
                    break;
            }
            break;
        case T_ID:
            //eh bello...
            break;
        case T_ATOMIC_DOMAIN:
            //CHAR, INT, REAL, STRING, BOOL
            node = new_schema_node(dom_child->value.ival);
            break;
        default:
            break;
    }
    return node;
}

Pschema new_schema_node(int _type){
    Pschema node = (Pschema) malloc(sizeof(Schema));
    node->type = _type;
    return node;
}

Pschema new_struct_schema(Pnode s){
    
}

/*
 typedef struct sschema{
 int type; //CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR, ATTR
 char * id; //field name
 int size; //array size (type = VECTOR)
 struct sschema *p1, *p2;
 } Schema, * Pschema;
 */

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