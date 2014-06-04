#include "symbol_table.h"
#include "parser.h"

int oid = 1;

void handle_function_part(Pnode current, Phash_node func, int loc_oid, Class part_class){
    if (current->child != NULL) {
        Pnode child;
        child = current->child;
        while (child != NULL) {
            Pnode id_list = child->child;
            Pschema domain_sch = create_schema(id_list->brother);
            
            Pnode id = id_list->child;
            while (id != NULL){
                Phash_node id_node = new_id_node(id->value.sval, part_class, loc_oid);
                loc_oid++;
                id_node->schema = domain_sch;
                insert(id_node, func->locenv);
                id = id->brother;
            }
            child = child->brother;
        }
    }
}

Phash_node create_symbol_table(Pnode root, Phash_node father){
    Pnode current, child;
    switch (root->type) {
        case T_NONTERMINAL:
            switch (root->value.ival) {
                case NPROGRAM:
                    return create_symbol_table(root->child, father);
                    break;
                case NFUNC_DECL:
                    current = root->child; //ID
                    
                    Phash_node func = new_function_node(current->value.sval);
                    func->father = father;
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
                    
                    current = current->brother; //TYPE_SECT_OPT
                    handle_function_part(current, func, loc_oid, CLTYPE);

                    
                    current = current->brother; //VAR_SECT_OPT
                    handle_function_part(current, func, loc_oid, CLVAR);

                    
                    current = current->brother; //CONST_SECT_OPT
                    handle_function_part(current, func, loc_oid, CLCONST);

                    
                    current = current->brother; //FUNC_LIST_OPT
                    if (current->child != NULL) {
                        child = current->child; // FUNC DECL
                        while (child != NULL) {//loop on FUNC DECL
                            insert(create_symbol_table(current, func), func->locenv);
                            child = child->brother;
                        }
                    }
                    
                    
                    print_func_node(func);
                    
                    return func;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return NULL;
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
                            Pschema to_add = create_domain_schema(decl_domain, id->value.sval);
                            if(last == NULL){
                                node->p1 = to_add;
                                last = node->p1;
                            }
                            else{
                                last->p2 = to_add;
                                last = last->p2;
                            }
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