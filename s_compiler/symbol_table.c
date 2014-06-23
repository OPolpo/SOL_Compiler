#include "symbol_table.h"
#include "parser.h"
#include "semantic.h"

int oid = 1;
char error_msg_symb[100];

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
                if(!insert(id_node, (func->aux)->locenv)){
                    sprintf(error_msg_symb, "ID \"%s\" already defined in same environment\n", id->value.sval);
                    sem_error(id, error_msg_symb);
                }
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
                    (func->aux)->locenv = loc;
                    (func->aux)->last_oid = 1;
                    
                    (func->aux)->formals_num = 0;
                    current = current->brother; //DECL_LIST_OPT
                    
                    if (current->child != NULL) {//handle parameters
                        child = current->child; //DECL
                        Formal * last_formal = (func->aux)->formal;
                        while (child != NULL) { //loop on DECL
                            Pnode id_list = child->child;
                            Pschema domain_sch = create_schema(id_list->brother, func, NULL);
                            
                            Pnode id = id_list->child;
                            while (id != NULL){ //loop on IDs
                                Phash_node id_node = new_id_node(id->value.sval, CLPAR, ((func->aux)->last_oid));
                                ((func->aux)->last_oid)++;//non sono convinto
                                id_node->schema = domain_sch;
                                if (!insert(id_node, (func->aux)->locenv)) {
                                    sprintf(error_msg_symb, "Name of formal parameters must be unique, \"%s\" already declared\n", id->value.sval);
                                    sem_error(id, error_msg_symb);
                                }
                                
                                Formal * to_add = (Formal *)calloc(1,sizeof(Formal));
                                to_add->formal = id_node;
                                
                                if(last_formal==NULL){
                                    (func->aux)->formal=to_add;
                                    //printf("last_formal==NULL %d\n", func->formal);
                                }
                                else {
                                    last_formal->next=to_add;
                                    //printf("last_formal!=NULL %d\n", id_node->oid);
                                }
                                last_formal = to_add;
                                
                                (func->aux)->formals_num++;//on single ID
                                id = id->brother;
                            }
                            child = child->brother; //next DECL
                        }
                        
                    }
                    
                    current = current->brother; //DOMAIN
                    func->schema = create_schema(current, func->father, NULL);
                    
                    current = current->brother; //TYPE_SECT_OPT
                    handle_function_part(current, func, &((func->aux)->last_oid), CLTYPE);
                    
                    current = current->brother; //VAR_SECT_OPT
                    handle_function_part(current, func, &((func->aux)->last_oid), CLVAR);
                    
                    current = current->brother; //CONST_SECT_OPT
                    handle_function_part(current, func, &((func->aux)->last_oid), CLCONST);
                    
                    print_func_node(func);
                    printSchema(func->schema," ");
                    print_hash_content((func->aux)->locenv);
                    
                    current = current->brother; //FUNC_LIST_OPT
                    if (current->child != NULL) {
                        child = current->child; // FUNC DECL
                        while (child != NULL) {//loop on FUNC DECL
                            if (!insert(create_symbol_table(child, func), (func->aux)->locenv)) {
                                sprintf(error_msg_symb, "Function names must be unique, \"%s\" already declared\n", child->value.sval);
                                sem_error(child, error_msg_symb);
                            }
                            child = child->brother;
                        }
                    }
                    return func;
                    break;
                default:
                    printf("ERROR: create_symbol_table called on wrong nonterminal\n");
                    break;
            }
            break;
        default:
            printf("ERROR: create_symbol_table called on a terminal\n");
            break;
    }
    printf("ERROR: something wrong occurred in create_symbol_table\n");
    return NULL;
}

Phash_node new_function_node(char * _name){
    Phash_node node = (Phash_node) calloc (1,sizeof(Hash_node));
    node->aux = calloc(1, sizeof(Aux_fun_param));
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
    Pschema node = NULL;
    Phash_node type_decl = NULL;
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
                                Pschema temp = node->p1;
                                while (temp != NULL){
                                    if (strcmp(to_add->id, temp->id)==0){
                                        sprintf(error_msg_symb, "Attributes in STRUCT decl must be unique, \"%s\" already present\n",to_add->id);
                                        sem_error(id,error_msg_symb);
                                    }
                                    temp = temp->p2;
                                }
                                
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
                type_decl = getNode(dom_child->value.sval, (func->aux)->locenv);
                if (type_decl != NULL){
                    node = type_decl->schema;
                    break;
                }
                func = func->father;
            }
            if (type_decl == NULL){
                sprintf(error_msg_symb, "ERROR type not found: %s\n", dom_child->value.sval);
                sem_error(dom_child, error_msg_symb);
            }
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

int are_compatible(Pschema a, Pschema b){
    int ok;
    if (a == NULL || b == NULL) {
        return 0;
    }
    switch (a->type) {//CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR
        case CHAR:
        case INT:
        case REAL:
        case BOOL:
        case STRING:
            return (a->type == b->type);
            break;
        case STRUCT:
            if (b->type == STRUCT) {
                Pschema a_child = a->p1;
                Pschema b_child = b->p1;
                while (a_child != NULL && b_child != NULL) {
                    ok = are_compatible(a_child, b_child);
                    a_child = a_child->p2;
                    b_child = b_child->p2;
                    if(!ok)
                        return 0; //FALSE
                }
                if (a_child != NULL || b_child != NULL) {
                    return 0; //FALSE
                }
                return 1;
            }
            return 0;
            break;
        case VECTOR:
            if (a->size != b->size) {
                return 0; //FALSE
            }
            return are_compatible(a->p1, b->p1);
        default:
            printf("default?!\n");
            return 0;
            break;
    }
    //return 0;
}

int compute_size(Pschema schema){
    switch(schema->type){
        case CHAR:
        case BOOL:
            return sizeof(char);
            break;
        case INT:
            return sizeof(int);
            break;
        case REAL:
            return sizeof(float);
            break;
        case STRING:
            return sizeof(char*);
            break;
        case VECTOR:
            return (schema->size)*compute_size(schema->p1);
            break;
        case STRUCT:{
            Pschema current = schema->p1;
            int dimension = 0;
            while(current){
                dimension += compute_size(current);
                current = current->p2;
            }
            return dimension;
        }
            break;
        default:
            fprintf(stderr, "%s\n","Some weird schema node in schema");
            return 0;
            break;
    }
}

char * schema2format(Pschema schema){
    char * str, * tmp_str, * tmp2_str; //mallocami
    Pschema tmp_schema;
    int i;
    switch (schema->type) {
        case CHAR:
            //str = (char *)malloc(2*sizeof(char));
            str = "c";
            break;
        case INT:
            //str = (char *)malloc(2*sizeof(char));
            str = "i";
            break;
        case REAL:
            //str = (char *)malloc(2*sizeof(char));
            str = "r";
            break;
        case STRING:
            //str = (char *)malloc(2*sizeof(char));
            str = "s";
            break;
        case BOOL:
            //str = (char *)malloc(2*sizeof(char));
            str = "b";
            break;
        case VECTOR:
            tmp_str = schema2format(schema->p1);
            i=log10(schema->size);
            str = (char *)calloc((strlen(tmp_str)+i+1+3+1),sizeof(char));
            sprintf(str, "[%d,%s]", schema->size, tmp_str);
            break;
        case STRUCT:
            tmp_schema = schema->p1;
            tmp_str = schema2format(tmp_schema);
            str = calloc((strlen(tmp_schema->id)+strlen(tmp_str)+5),sizeof(char));
            sprintf(str, "(%s:%s", tmp_schema->id, tmp_str);
            
            tmp_schema = tmp_schema->p2;
            while (tmp_schema) {
                tmp_str = schema2format(tmp_schema);
                //printf("--->%s\n", tmp_str);
                
                tmp2_str = calloc((strlen(str)+strlen(tmp_str)+1+2),sizeof(char));
                strcpy(tmp2_str, str);
                str = tmp2_str;
                strcat(str, ",");
                strcat(str, (tmp_schema->id? tmp_schema->id: ""));
                strcat(str, ":");
                strcat(str, tmp_str);//
                ///sprintf(str, ",%s:%s", tmp_schema->id, tmp_str);
                
                tmp_schema = tmp_schema->p2;
            }
            strcat(str, ")");
            //sprintf(str, ")");
            break;
        default:
            str = NULL;
            break;
    }
    return str;
}

char * make_format(Pschema schema){
    char * str, * tmp_str;
    tmp_str = schema2format(schema);
    str = calloc(strlen(tmp_str)+3, sizeof(char));
    sprintf(str, "\"%s\"", tmp_str);
    return str;
}

