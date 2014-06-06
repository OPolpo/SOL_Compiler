#include "symbol_table.h"
#include "parser.h"

int oid = 1;

void handle_function_part(Pnode current, Phash_node func, int * loc_oid, Class part_class){
    if (current->child != NULL) { //?_SECT_OPT
        Pnode child;
        child = current->child; //DECL
        while (child != NULL) {
            Pnode id_list = child->child;
            Pschema domain_sch = create_schema(id_list->brother, func, NULL);
            
            Pnode id = id_list->child;
            while (id != NULL){
                Phash_node id_node = new_id_node(id->value.sval, part_class, *loc_oid);
                (*loc_oid)++;
                id_node->schema = domain_sch;
                insert(id_node, func->locenv);
                id = id->brother;
            }
            child = child->brother;
            if (part_class == CLCONST)
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
                            Pschema domain_sch = create_schema(id_list->brother, func, NULL);
                            
                            Pnode id = id_list->child;
                            while (id != NULL){ //loop on IDs
                                Phash_node id_node = new_id_node(id->value.sval, CLPAR, loc_oid);
                                loc_oid++;
                                id_node->schema = domain_sch;
                                insert(id_node, func->locenv);
                                
                                Formal * to_add = (Formal *)calloc(1,sizeof(Formal));
                                to_add->formal = id_node;
                                
                                if(last_formal==NULL){
                                    func->formal=to_add;
                                    //printf("last_formal==NULL %d\n", func->formal);
                                }
                                else {
                                    last_formal->next=to_add;
                                    //printf("last_formal!=NULL %d\n", id_node->oid);
                                }
                                last_formal = to_add;
                                
                                func->formals_num++;//on single ID
                                id = id->brother;
                            }
                            child = child->brother; //next DECL
                        }
                        
                    }
                    
                    current = current->brother; //DOMAIN
                    func->schema = create_schema(current, func->father, NULL);
                    
                    current = current->brother; //TYPE_SECT_OPT
                    handle_function_part(current, func, &loc_oid, CLTYPE);
                    
                    current = current->brother; //VAR_SECT_OPT
                    handle_function_part(current, func, &loc_oid, CLVAR);
                    
                    current = current->brother; //CONST_SECT_OPT
                    handle_function_part(current, func, &loc_oid, CLCONST);
                    
                    print_func_node(func);
                    printSchema(func->schema,"");
                    print_hash_content(func->locenv);
                    
                    current = current->brother; //FUNC_LIST_OPT
                    if (current->child != NULL) {
                        child = current->child; // FUNC DECL
                        while (child != NULL) {//loop on FUNC DECL
                            
                            insert(create_symbol_table(child, func), func->locenv);
                            child = child->brother;
                        }
                    }
                    
                    
                    
                    
                    return func;
                    break;
                case NDECL_LIST_OPT:
                    printf("0");
                    break;
                case NDECL:
                    printf("1");
                    break;
                case NID_LIST:
                    printf("2");
                    break;
                case NDOMAIN:
                    printf("3");
                    break;
                case NSTRUCT_DOMAIN:
                    printf("4");
                    break;
                case NVECTOR_DOMAIN:
                    printf("5");
                    break;
                case NTYPE_SECT_OPT:
                    printf("6");
                    break;
                case NVAR_SECT_OPT:
                    printf("7");
                    break;
                case NCONST_SECT_OPT:
                    printf("8");
                    break;
                case NFUNC_LIST_OPT:
                    printf("9");
                    break;
                    
                default:
                    break;
            }
            
            
            break;
        default:
            break;
    }
    printf("something wrong occurred\n");
    return NULL;
    /*
     if (root == NULL) {
     return Phash_node;
     }
     */
}

Phash_node new_function_node(char * _name){
    Phash_node node = (Phash_node) calloc (1,sizeof(Hash_node));
    node->name = _name;
    node->oid = oid;
    oid++;
    node->class_node = CLFUNC;
    return node;
}

Phash_node new_id_node(char * _name, Class _class, int loc_oid){
    Phash_node node = (Phash_node) calloc (1,sizeof(Hash_node));
    node->name = _name;
    node->oid = loc_oid;
    node->class_node = _class;
    return node;
}

Pschema create_schema(Pnode domain, Phash_node func, char * id){
    //func: function node of the local environment
    Pnode dom_child = domain->child;
    Pschema node;
    Phash_node type_decl;
    switch (dom_child->type) {
        case T_NONTERMINAL:
            switch (dom_child->value.ival) {
                case NSTRUCT_DOMAIN:
                    node = new_schema_node(STRUCT);
                    node->id = id;
                    Pnode decl = dom_child->child;
                    
                    Pschema last = node->p1;
                    while (decl != NULL) { //DECL
                        Pnode decl_domain = decl->child->brother;
                        Pnode id = decl->child->child;
                        
                        
                        while (id != NULL) {
                            Pschema to_add = create_schema(decl_domain, func, id->value.sval);
                            if(last == NULL){
                                node->p1 = to_add;
                                //last = node->p1;
                            }
                            else{
                                last->p2 = to_add;
                                //last = last->p2;
                            }
                            last = to_add;
                            id = id->brother;
                        }
                        decl = decl->brother;
                    }
                    break;
                case NVECTOR_DOMAIN:
                    node = new_schema_node(VECTOR);
                    node->id = id;
                    node->size = dom_child->child->value.ival;
                    node->p1 = create_schema(dom_child->child->brother, func, NULL);
                    break;
                default:
                    break;
            }
            break;
        case T_ID:
            while (func != NULL) {
                type_decl = getNode(dom_child->value.sval, func->locenv);
                if (type_decl != NULL){
                    node = type_decl->schema;
                    break;
                }
                func = func->father;
            }
            if (type_decl == NULL) printf("ERRORE type not found: %s\n", dom_child->value.sval);
            break;
        case T_ATOMIC_DOMAIN:
            //CHAR, INT, REAL, STRING, BOOL
            node = new_schema_node(dom_child->value.ival);
            node->id = id;
            break;
        default:
            break;
    }
    return node;
}

Pschema new_schema_node(int _type){
    Pschema node = (Pschema) calloc(1,sizeof(Schema));
    node->type = _type;
    return node;
}

void printSchema(Pschema root, char* father_indent){
    if(root==NULL) return;

    int i;
    Pschema p;

    //--Indent Stuff
    char* my_indent = calloc(1000,sizeof(char));
    my_indent[0]=0;
    strcpy(my_indent, father_indent);

    if(root->p2)
        //strcat(my_indent,"    ├");
        strcat(my_indent,"├");
    else
        //strcat(my_indent,"    └");
        strcat(my_indent,"└");
    printf("%s─", my_indent);

    //--PRINTING SINGLE NODE
    switch(root->type){
        case VECTOR:
            printf("%s [ %s ] [ %d ]", "VECTOR", root->id, root->size);
            break;
        case STRUCT:
            printf("%s [ %s ] [ - ]", "STRUCT", root->id);
            break;
        case CHAR:
            printf("%s [ %s ] [ - ]", "CHAR", root->id);
            break;
        case INT:
            printf("%s [ %s ] [ - ]", "INT", root->id);
            break;
         case REAL:
            printf("%s [ %s ] [ - ]", "REAL", root->id);
            break;
        case STRING:
            printf("%s [ %s ] [ - ]", "STRING", root->id);
            break;
        case BOOL:
            printf("%s [ %s ] [ - ]", "BOOL", root->id);
            break;
        // case ATTR:
        //     printf(" %s [ - ] [ - ]", "ATTR");
        //     break;
        default:
            printf("ERROR (maybe ID)\n");
            break;    
    }
    
    //--OTHER INDENT AND RECURSION AND BROTHER CICLE
    printf("\n");
    for(p=root->p1; p != NULL; p = p->p2){
        my_indent[strlen(my_indent)-4]=0;
        strcat(my_indent," |  ");
        if(root->p2 == NULL){
            my_indent[strlen(my_indent)-4]=0;
            strcat(my_indent,"    ");
        }
        printSchema(p, my_indent);
    }
    free(my_indent);
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