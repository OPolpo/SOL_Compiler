#include "semantic.h"
#include "parser.h"
#include "tree.h"

#define VERBOSE 1

char error_msg[100];
char convert_bool[] = {'0','1'};

int sem_program(Pnode root, Phash_node f_loc_env, Code * code){
    Poid2address * func_table = new_o2a_table();
    
#if VERBOSE
    printf("@@ in sem_program\n");
#endif
    int not_first = 0;
    Code func_decl_code = makecode(S_NOOP);
    int ok = sem_func_decl(root->child, f_loc_env, not_first, &func_decl_code, func_table);
    
    Code param_code = makecode(S_NOOP);
    Formal * current_formal = f_loc_env->aux->formal;
    while(current_formal){
        param_code = appcode(makecode_str(S_RD, make_format(current_formal->formal->schema)), param_code);
        current_formal = current_formal->next;
    }
    *code = appcode(*code, param_code);
    
    Code write_return = makecode_str(S_WRITE, make_format(f_loc_env->schema));
    
    *code = concode(makecode1(S_SCODE, func_decl_code.size+5+f_loc_env->aux->formals_num),
                    param_code,
                    make_push_pop(f_loc_env->aux->formals_num,f_loc_env->aux->num_obj, -1, f_loc_env->oid),
                    write_return,
                    makecode(S_HALT),
                    func_decl_code,
                    endcode());
    
    cleanup_goto(code, func_table);
    destroy_o2a(func_table);
    return ok;
}

int sem_func_decl(Pnode root, Phash_node f_loc_env, int not_first, Code * code, Poid2address * func_table){
#if VERBOSE
    printf("@@ in sem_func_decl\n");
#endif
    Pnode id = root->child;
	Pnode current = id->brother;
    
    Phash_node new_f_loc_env;
    if (not_first)
        new_f_loc_env = getNode(id->value.sval, f_loc_env->aux->locenv);
    else
        new_f_loc_env = f_loc_env;
    *code = appcode(*code, makecode1(S_FUNC, new_f_loc_env->oid));
    insert_o2a(new_o2a(new_f_loc_env->oid, &(code->tail->address)),func_table);
	current = current->brother;
    
    Pschema domain_schema = new_schema_node(-1);
	int domain_ok = sem_domain(current, new_f_loc_env, &domain_schema, code);
	current = current->brother;
    
	current = current->brother;// with this we skip type_sec_op cause there are not control or code action to do
    
    int var_num_objects = 0;
    Code var_const_code = makecode(S_NOOP);
	int var_sect_opt_ok = sem_var_sect_opt(current, new_f_loc_env, &var_const_code, &var_num_objects);
    current = current->brother;
    
    int const_num_objects = 0;
    int const_sect_opt_ok = sem_const_sect_opt(current, new_f_loc_env, &var_const_code, &const_num_objects);
	current = current->brother;
    
    Code function_list_code = makecode(S_NOOP);
	int func_list_opt_ok = sem_func_list_opt(current, new_f_loc_env, &function_list_code, func_table);
	current = current->brother;
    
    Code code_new_aux = makecode(S_NOOP);

    Stat * start = code->tail; //we save the pointer to first stat of the code to sanitize // this is for optimixe sanitization process of return
    int code_len = code->size; //we save the size of the code at the start point // this is for optimixe sanitization process of return
    Code func_body_code = makecode(S_NOOP);
	int func_body_ok = sem_func_body(current, new_f_loc_env, &func_body_code, &code_new_aux); //during sem_func_body, the code_new_aux will be modified
    
    *code = appcode(*code, var_const_code);
    *code = appcode(*code, code_new_aux);
    *code = appcode(*code, func_body_code);
    
    cleanup_return(start, code_len, code); //sanitize
    *code = appcode(*code, function_list_code);
    
    return /*decl_list_opt_ok &&*/ domain_ok && var_sect_opt_ok && const_sect_opt_ok && func_list_opt_ok && func_body_ok;
}

int sem_decl_list_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects){
#if VERBOSE
    printf("@@ in sem_decl_list_opt\n");
#endif
    Pnode current = root->child;
    int decl_list_opt_ok = 1;
    while(current != NULL){
        int decl_num_objects = 0;
        Pschema domain_schema = new_schema_node(-1);
        decl_list_opt_ok = decl_list_opt_ok && sem_decl(current, f_loc_env, &domain_schema, code, &decl_num_objects);
        (*num_objects) += decl_num_objects;
        current = current->brother;
    }
    return decl_list_opt_ok;
}

int sem_decl(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int * num_objects){
#if VERBOSE
    printf("@@ in sem_decl\n");
#endif
    Pnode domain_node = root->child->brother;
    int ok = sem_domain(domain_node, f_loc_env, stype, code);
    sem_id_list(root->child, num_objects);
    int i;
    if((*stype)->type==SCSTRUCT || (*stype)->type==SCVECTOR)
        for(i = 0; i< *num_objects; i++)
            *code = appcode(*code, makecode1(S_NEWS,compute_size(*stype)));
    else
        for(i = 0; i< *num_objects; i++)
            *code = appcode(*code, makecode1(S_NEW,compute_size(*stype)));
    return ok;
}

int sem_id_list(Pnode root, int * num_objects){
#if VERBOSE
    printf("@@ in sem_id_list\n");
#endif
    Pnode current = root->child;
    while(current){
        (*num_objects)++;
        current = current->brother;
    }
    return 1;
}

int sem_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_domain\n");
#endif
    int ok = 1;
    Pnode dom_node = root->child;
    Phash_node h_node;
    
    switch (dom_node->type) {
        case T_NONTERMINAL:
            switch (dom_node->value.ival) {
                case NSTRUCT_DOMAIN:
                    ok = ok && sem_struct_domain(dom_node, f_loc_env, stype, code);
                    break;
                case NVECTOR_DOMAIN:
                    ok = ok && sem_vector_domain(dom_node, f_loc_env, stype, code);
                    break;
                default:
                    break;
            }
            break;
        case T_ID:
#if VERBOSE
            printf("@@ T_ID\n");
#endif
            int offset;
            h_node = find_visible_node(dom_node->value.sval, f_loc_env, &offset);
            ok = ok && (h_node != NULL && h_node->class_node == CLTYPE);
            if (!ok) {
                sprintf(error_msg, "Type error, type \"%s\" not defined or not visible\n", dom_node->value.sval);
                sem_error(dom_node, error_msg);
            }
            *stype = h_node->schema;
            break;
        case T_ATOMIC_DOMAIN:
#if VERBOSE
            printf("@@ T_ATOMIC_DOMAIN\n");
#endif
            switch (dom_node->value.ival) {
                case INT:
                    (*stype)->type = SCINT;
                    break;
                case REAL:
                    (*stype)->type = SCREAL;
                    break;
                case STRING:
                    (*stype)->type = SCSTRING;
                    break;
                case CHAR:
                    (*stype)->type = SCCHAR;
                    break;
                case BOOL:
                    (*stype)->type = SCBOOL;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return 1;
}
int sem_struct_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_struct_domain\n");
#endif
    int ok =1;
    Pnode decl = root->child;
    (*stype)->type = SCSTRUCT;
    Pschema last = (*stype)->p1;
    
    while (decl != NULL) { //DECL
        Pnode decl_domain = decl->child->brother;
        Pnode id = decl->child->child;
        
        while (id != NULL) {
            Pschema to_add = new_schema_node(-1);
            ok = ok && sem_domain(decl_domain, f_loc_env, &to_add, code);
            
            if(last == NULL){
                (*stype)->p1 = to_add;
            }
            else{
                last->p2 = to_add;
            }
            last = to_add;
            id = id->brother;
        }
        decl = decl->brother;
    }
    return ok;
}
int sem_vector_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_vector_domain\n");
#endif
    int ok =1;
    (*stype)->type = SCVECTOR;
    (*stype)->size = root->child->value.ival;
    (*stype)->p1 = new_schema_node(-1);
    ok = ok && sem_domain(root->child->brother, f_loc_env, &(*stype)->p1, code);
    return ok;
}

int sem_var_sect_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects){
#if VERBOSE
    printf("@@ in sem_var_sect_opt\n");
#endif
    Pnode current = root->child;
    int var_sect_opt_ok = 1;
    while(current != NULL){
        int var_num_objects = 0;
        Pschema domain_schema = new_schema_node(-1);
        var_sect_opt_ok = var_sect_opt_ok && sem_decl(current, f_loc_env, &domain_schema, code, &var_num_objects);
        (*num_objects) += var_num_objects;
        current = current->brother;
    }
    return 1;
}

int sem_const_sect_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects){
#if VERBOSE
    printf("@@ in sem_const_sect_opt\n");
#endif
    int ok = 1;
    if (root->child != NULL) {
        Pnode decl_node = root->child;
        Pnode expr_node;
        Code expr_code = makecode(S_NOOP);
        Phash_node id_h_node;
        while (decl_node != NULL) {
            expr_node = decl_node->brother;
            Pschema domain_schema = new_schema_node(-1);
            int const_num_objects = 0;
            ok = ok && sem_decl(decl_node, f_loc_env, &domain_schema, code, &const_num_objects);
            (*num_objects) += const_num_objects;
            
            Pschema expr_schema = new_schema_node(-1);
            Pnode id_node = decl_node->child->child;
            while (id_node) {
                ok = ok && sem_expr(expr_node, f_loc_env, &expr_schema, &expr_code, 0);
                id_h_node = getNode(id_node->value.sval, f_loc_env->aux->locenv);
                expr_code = appcode(expr_code, makecode2(S_STO, 0, id_h_node->oid));
                id_node = id_node->brother;
            }
            ok = ok && are_compatible(domain_schema, expr_schema);
            if(!ok){
                sem_error(decl_node, "Type error, in CONST initialization domain and expression must be compatible\n");
            }
            decl_node = expr_node->brother;
        }
        *code = appcode(*code,expr_code);
    }
    return ok;
}

int sem_func_list_opt(Pnode root, Phash_node f_loc_env, Code * code, Poid2address * func_table){
#if VERBOSE
    printf("@@ in sem_func_list_opt\n");
#endif
    int ok = 1;
    Pnode func_decl_node = root->child;
    if (func_decl_node == NULL) {
        return ok;
    }
    while (func_decl_node != NULL) {
        ok = ok && sem_func_decl(func_decl_node, f_loc_env, 1, code, func_table);
        func_decl_node = func_decl_node->brother;
    }
    return ok;
}

int sem_func_body(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_func_body\n");
#endif
    Pnode id1 = root->child;
    Pnode stat_list_node = id1->brother;
    Pnode id2 = stat_list_node->brother;
    int ok = 1;
    int return_ok = 0;
    
    ok = (strcmp(f_loc_env->name, id1->value.sval) == 0);
    if (!ok) {
        sem_error(id1, "Function ID different from ID in function body BEGIN\n");
    }
    ok = ok && sem_stat_list(stat_list_node, f_loc_env, &return_ok, code, code_new_aux);
    
    ok = ok && (strcmp(id1->value.sval, id2->value.sval) == 0);
    if (!ok) {
        sem_error(id2, "Function ID different from ID in function body END\n");
    }
    
    if (!return_ok) {
        sem_error(id2, "Control may reach end of FUNC without a RETURN\n");
    }
    return ok;
}

int sem_stat_list(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_stat_list\n");
#endif
    Pnode stat_node = root->child;
    int ok = 1;
    int w_return_stat = 0;
    
    while(stat_node != NULL){
        ok = ok && sem_stat(stat_node, f_loc_env, &w_return_stat, code, code_new_aux);
        stat_node = stat_node->brother;
        *w_return = *w_return || w_return_stat;
    }
    return ok;
}

int sem_stat(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_stat\n");
#endif
    int ok = 1;
    int return_ok;
    Pnode child = root->child;
    
    switch (child->value.ival) {//always a NONTERMINAL
        case NASSIGN_STAT:
            ok = sem_assign_stat(child, f_loc_env, code);
            break;
        case NIF_STAT:
            ok = sem_if_stat(child, f_loc_env, &return_ok, code,  code_new_aux);
            *w_return = return_ok;
            break;
        case NWHILE_STAT:
            ok = sem_while_stat(child, f_loc_env, code, code_new_aux);
            break;
        case NFOR_STAT:
            ok = sem_for_stat(child, f_loc_env, code, code_new_aux);
            break;
        case NFOREACH_STAT:
            ok = sem_foreach_stat(child, f_loc_env, code, code_new_aux);
            break;
        case NRETURN_STAT:
            ok = sem_return_stat(child, f_loc_env, code);
            *w_return = 1;
            break;
        case NREAD_STAT:
            ok = sem_read_stat(child, f_loc_env, code);
            break;
        case NWRITE_STAT:
            ok = sem_write_stat(child, f_loc_env, code);
            break;
        default:
            ok = 0;
            sem_error(child, "Some weird nonterminal node in stat\n");
            break;
    }
    return ok;
}

int sem_assign_stat(Pnode root, Phash_node f_loc_env, Code * code){
#if VERBOSE
    printf("@@ in sem_assign_stat\n");
#endif
    Pnode lhs_node = root->child;
    Pnode expr_node = lhs_node->brother;
    int ok = 1;
    
    Pschema lhs_schema;
    Class lhs_class = -1; //maybe is not allocated...
    Pschema expr_schema = new_schema_node(-1);
    
    int is_s = 0;
    Code lhs_code = makecode(S_NOOP);
    ok = sem_left_hand_side(lhs_node, f_loc_env, &lhs_schema, &lhs_class, &lhs_code, 1, &is_s);
    ok = ok && (lhs_class == CLVAR || lhs_class == CLPAR); //not a CONST
    if (!ok) {
        sem_error(root, "Cannot assign value to a CONST\n");
    }
    
    Code expr_code = makecode(S_NOOP);
    ok = ok && sem_expr(expr_node, f_loc_env, &expr_schema, &expr_code, 0);
    
    ok = ok && are_compatible(lhs_schema, expr_schema);
    
    if (!ok) {
        sem_error(root, "Type error in ASSIGNMENT, type must be compatible\n");
    }
    
    if (is_s) {
        *code = concode(*code,
                        lhs_code,
                        expr_code,
                        makecode(S_IST),
                        endcode());
    }else{
        *code = concode(*code,
                        expr_code,
                        lhs_code,
                        endcode());
    }
    return ok;
}

int sem_left_hand_side(Pnode root, Phash_node f_loc_env, Pschema * stype, Class * lhs_class, Code * code, int is_addr, int * is_s){//to be called on a SCHEMA not mallocated
#if VERBOSE
    printf("@@ in sem_left_hand_side\n");
#endif
    Phash_node h_node;
    int lhs_ok = 1;
    Pnode child = root->child;
    
    switch (child->type) {
        case T_ID:{
            int offset;
            h_node = find_visible_node(child->value.sval, f_loc_env, &offset);
            if (h_node == NULL) {
                lhs_ok = 0;
                sprintf(error_msg, "Use of not visible ID \"%s\"\n", child->value.sval);
                sem_error(child, error_msg);
            }
            lhs_ok = (h_node->class_node == CLVAR || h_node->class_node == CLPAR || h_node->class_node == CLCONST || h_node->class_node == CLCOUNT);
            if (!lhs_ok) {
                sem_error(child, "An lhs cannot be a function or a type name\n");
            }
            *lhs_class = h_node->class_node;
            *stype = h_node->schema;
            
            if (*is_s) {
                *code = appcode(*code, makecode2(S_LDA,offset,h_node->oid));
            }else
                if (is_addr){
                    *code = appcode(*code, makecode2(S_STO,offset,h_node->oid));
                }else{
                    *code = appcode(*code, makecode2(S_LOD,offset,h_node->oid));
                }
        }
            break;
        case T_NONTERMINAL:
            switch (child->value.ival) {
                case NFIELDING:
                    lhs_ok = sem_fielding(child, f_loc_env, stype, lhs_class, code, is_addr, is_s);
                    break;
                case NINDEXING:
                    lhs_ok = sem_indexing(child, f_loc_env, stype, lhs_class, code, is_addr, is_s);
                    break;
                default:
                    sem_error(child, "Some weird nonterminal node in lhs\n");
                    break;
            }
            break;
        default:
            lhs_ok = 0;
            sem_error(child, "Some weird terminal node in lhs\n");
            break;
    }
    return lhs_ok;
}

int sem_fielding(Pnode root, Phash_node f_loc_env, Pschema * stype, Class * lhs_class, Code * code, int is_addr, int * is_s){
#if VERBOSE
    printf("@@ in sem_fielding\n");
#endif
    int first_field = !(*is_s);
    *is_s = 1;
    
    int ok_field;
    Pnode lhs_node = root->child;
    Pschema lhs_type = *stype;
    
    ok_field = sem_left_hand_side(lhs_node, f_loc_env, &lhs_type, lhs_class, code, is_addr, is_s);
    
    printSchema(lhs_type, " ");
    
    ok_field = ok_field && (lhs_type->type == SCSTRUCT);
    if (!ok_field) {
        sem_error(root, "Type error, cannot use . on a lhs that is not a STRUCT\n");//to_do
    }
    //lhs is a STRUCT
    Pnode id_node = lhs_node->brother;
    //lhs.id must exist... so check lhs children
    Pschema lhs_attr = lhs_type->p1;
    Code end_code;
    int found = 0;
    int field_offset = 0;
    while (found==0 && lhs_attr != NULL) {
        if (strcmp(id_node->value.sval, lhs_attr->id)==0) {
            memcpy(*stype, lhs_attr, sizeof(Schema));
            (*stype)->p2=NULL;
            found = 1;
            if (!is_addr && first_field) {
                if (lhs_attr->type == SCVECTOR || lhs_attr->type == SCSTRUCT) {
                    end_code = makecode1(S_SIL, compute_size(lhs_attr));
                }else{
                    end_code = makecode1(S_EIL, compute_size(lhs_attr));
                }
            }
        }else{
            field_offset += compute_size(lhs_attr);
        }
        lhs_attr = lhs_attr->p2;
    }
    *code = appcode(*code, makecode1(S_FDA, field_offset));
    if (!is_addr && first_field) {
        *code = appcode(*code, end_code);
    }
    ok_field = ok_field && found;
    if (!found) {
        sprintf(error_msg, "Semantic error, trying to access a non-existent field \"%s\"\n", id_node->value.sval);
        sem_error(root, error_msg);
    }
    return ok_field;
}

int sem_indexing(Pnode root, Phash_node f_loc_env, Pschema * stype, Class * lhs_class, Code * code, int is_addr, int * is_s){
#if VERBOSE
    printf("@@ in sem_indexing\n");
#endif
    int first_index = !(*is_s);
    *is_s = 1;
    
    int ok_index;
    Pnode lhs_node = root->child;
    Pnode index_node = lhs_node->brother;
    
    Pschema lhs_type = *stype;
    
    ok_index = sem_left_hand_side(lhs_node, f_loc_env, &lhs_type, lhs_class, code, is_addr, is_s);
    
    ok_index = ok_index && (lhs_type->type == SCVECTOR);
    if (!ok_index) {
        sem_error(root, "Semantic error, cannot index a non-VECTOR\n");//to_do
    }
    
    Pschema index_type = new_schema_node(-1);
    ok_index = ok_index && sem_expr(index_node, f_loc_env, &index_type, code, 0);
    ok_index = ok_index && (index_type->type == SCINT);
    if (!ok_index) {
        sem_error(root, "Semantic error, index must be of type INT\n");//to_do
    }
    
    *code = appcode(*code, makecode1(S_IXA, compute_size(lhs_type->p1)));
    if (!is_addr && first_index) {
        if (lhs_type->p1->type == SCVECTOR || lhs_type->p1->type == SCSTRUCT) {
            *code = appcode(*code, makecode1(S_SIL, compute_size(lhs_type->p1)));
        }else{
            *code = appcode(*code, makecode1(S_EIL, compute_size(lhs_type->p1)));
        }
    }
    *stype = lhs_type->p1;
    return ok_index;
}

int sem_if_stat(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_if_stat\n");
#endif
    
	Pnode main_expr_node = root->child;
	Pnode if_stat_list_node = main_expr_node->brother;
	Pnode elsif_stat_list_opt_node = if_stat_list_node->brother;
	Pnode else_stat_list_node = elsif_stat_list_opt_node->brother;//can be NULL
    
	//check constraint on conditional clause
	Pschema main_expr_type = new_schema_node(-1);;
	int main_expr_ok = sem_expr(main_expr_node, f_loc_env, &main_expr_type, code, 0);
    
	if (main_expr_type->type!=SCBOOL){
		sem_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}
    
    int return_if_stat_list = 0;
    int return_else_list = 0;
    int return_elsif_stat_list_opt = 0;
    
    Code if_stat_list_code = makecode(S_NOOP);
    int if_stat_list_ok = sem_stat_list(if_stat_list_node, f_loc_env, &return_if_stat_list, &if_stat_list_code, code_new_aux);
    
    Code else_stat_list_code = makecode(S_NOOP);
    int else_stat_list_ok = 1;
    int offset_to_exit;
    if (else_stat_list_node) {
        else_stat_list_ok = sem_stat_list(else_stat_list_node, f_loc_env, &return_else_list, &else_stat_list_code, code_new_aux);
        else_stat_list_code = appcode(makecode1(S_JMP, else_stat_list_code.size+1), else_stat_list_code);
        offset_to_exit = else_stat_list_code.size;
    }
    else {
        return_else_list = 1;
        offset_to_exit = 0;
    }
    
    
    Code elsif_stat_list_opt_code = makecode(S_NOOP);
	int elsif_stat_list_opt_ok = sem_elsif_stat_list_opt(elsif_stat_list_opt_node, f_loc_env, &return_elsif_stat_list_opt, &elsif_stat_list_opt_code, code_new_aux, &offset_to_exit);
    
    *w_return = return_if_stat_list && return_else_list && return_elsif_stat_list_opt;
    // if(elsif_stat_list_opt_node->child){
    //     printf("il blocco elsif ce quindi sommo %d",((elsif_stat_list_opt_node->child)? 2 : 1));
    // }

    *code = concode(*code,
                    makecode1(S_JMF, if_stat_list_code.size + ((elsif_stat_list_opt_node->child)? 2 : (else_stat_list_node ? 2 : 1))), //se manca il blocco elseif deve essere +1
                    if_stat_list_code,
                    elsif_stat_list_opt_code,
                    else_stat_list_code,
                    endcode());
    
	return main_expr_ok && if_stat_list_ok && elsif_stat_list_opt_ok && else_stat_list_ok;
}

int sem_elsif_stat_list_opt(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux, int * offset_to_exit){
#if VERBOSE
    printf("@@ in sem_elsif_stat_list_opt\n");
#endif
    if (root->child == NULL){
        *w_return = 1;
		return 1;
	}
    
	Pnode main_expr_node = root->child;
    Pnode stat_list_node;
	int stat_list_ok=0, main_expr_ok=0;
    
    Stack_node_code * top = NULL;
    Code * ptemp_code;
    
    *w_return = 1;
    while (main_expr_node) {
        stat_list_node = main_expr_node->brother;
        
        ptemp_code = (Code *)malloc(sizeof(Code));
        *ptemp_code = makecode(S_NOOP);
        
        //check constraint on conditional clause
        Pschema main_expr_type = new_schema_node(-1);;
        main_expr_ok = sem_expr(main_expr_node, f_loc_env, &main_expr_type, ptemp_code, 0);
        
        if (main_expr_type->type!=SCBOOL){
            sem_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
        }
        int return_stat = 0;
        Code stat_list_code = makecode(S_NOOP);
        stat_list_ok = sem_stat_list(stat_list_node, f_loc_env, &return_stat, &stat_list_code, code_new_aux);
        
        printf("\n\n offset_to_exit vale %d", *offset_to_exit);
        *w_return = return_stat && *w_return;
        *ptemp_code = concode(*ptemp_code,
                              makecode1(S_JMF, stat_list_code.size+((stat_list_node->brother)? 2 : (!offset_to_exit ? 1 : 2))),
                              stat_list_code,
                              endcode());
        StackPush(&top, ptemp_code);
        
        main_expr_node = stat_list_node->brother;
    }
    Code reverse = makecode(S_NOOP);
    while (top != NULL) {
        Code * current_code = StackPop(&top);
        (*offset_to_exit) += (current_code->size+1);
        reverse = concode(makecode1(S_JMP, *offset_to_exit),
                          *current_code,
                          reverse,
                          endcode());
    }
    *code = appcode(*code, reverse);
    
	return main_expr_ok && stat_list_ok;
}

int sem_while_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_while_stat\n");
#endif
    Pnode expr_node = root->child;
    Pnode stat_list_node = expr_node->brother;
    
    Pschema expr_schema = new_schema_node(-1);
    Code expr_code = makecode(S_NOOP);
    int ok = sem_expr(expr_node, f_loc_env, &expr_schema, &expr_code, 0);
    
    ok = ok && (expr_schema->type == SCBOOL);
    if(!ok){
        sem_error(root, "Type Error, expected BOOL in WHILE clause\n");
    }
    
    Code stat_list_code = makecode(S_NOOP);
    int not_used;
    ok = ok && sem_stat_list(stat_list_node, f_loc_env, &not_used, &stat_list_code, code_new_aux);
    
    *code = concode(*code,
                    expr_code,
                    makecode1(S_JMF, stat_list_code.size+2),
                    stat_list_code,
                    makecode1(S_JMP, -(stat_list_code.size+1+expr_code.size)),
                    endcode()
                    );
    return ok;
}

int sem_for_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_for_stat\n");
#endif
    Pnode id_node = root->child;
    Pnode expr1_node = id_node->brother;
    Pnode expr2_node = expr1_node->brother;
    Pnode stat_list_node = expr2_node->brother;
    int ok = 1;
    int offset;
    Phash_node id_hash_node = find_visible_node(id_node->value.sval, f_loc_env, &offset);
    if (id_hash_node == NULL) {
        ok = 0;
        sprintf(error_msg, "Variable ID \"%s\" in FOR-STAT is not defined\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    ok = ok && (id_hash_node->class_node == CLVAR || id_hash_node->class_node == CLPAR);
    if (!ok) {
        sprintf(error_msg, "Variable ID \"%s\" in FOR-STAT must be a VAR or a PAR\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    ok = ok && (id_hash_node->schema->type == SCINT);
    if (!ok) {
        sprintf(error_msg, "Variable ID \"%s\" in FOR-STAT must be of type INT\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    
    Pschema expr1_schema = new_schema_node(-1);
    Code expr1_code = makecode(S_NOOP);
    ok = ok && sem_expr(expr1_node, f_loc_env, &expr1_schema, &expr1_code, 0);
    ok = ok && (expr1_schema->type == SCINT);
    if (!ok) {
        sem_error(expr1_node, "Type error: expected INT in FOR-STAT\n");
    }
    Pschema expr2_schema = new_schema_node(-1);
    Code expr2_code = makecode(S_NOOP);
    ok = ok && sem_expr(expr2_node, f_loc_env, &expr2_schema, &expr2_code, 0);
    ok = ok && (expr2_schema->type == SCINT);
    if (!ok) {
        sem_error(expr2_node, "Type error: expected INT in FOR-STAT\n");
    }
    Class id_class = id_hash_node->class_node;
    id_hash_node->class_node = CLCOUNT;
    
    char * id_aux;
    asprintf(&id_aux, "0_AUX_%d", f_loc_env->aux->last_oid);
    Phash_node end_condition_expr_value = new_id_node(id_aux, CLCONST, f_loc_env->aux->last_oid);
    f_loc_env->aux->last_oid++;
    f_loc_env->aux->num_obj++;
    end_condition_expr_value->schema = new_schema_node(SCINT);
    insert(end_condition_expr_value, f_loc_env->aux->locenv);
    *code_new_aux = appcode(*code_new_aux, makecode1(S_NEW, sizeof(int)));
    
    int not_used;
    Code stat_list_code = makecode(S_NOOP);
    ok = ok && sem_stat_list(stat_list_node, f_loc_env, &not_used, &stat_list_code, code_new_aux);
    
    *code = concode(*code,
                    expr1_code,
                    makecode2(S_STO,offset,id_hash_node->oid),
                    expr2_code,
                    makecode2(S_STO,0,end_condition_expr_value->oid),
                    makecode2(S_LOD,offset,id_hash_node->oid),
                    makecode2(S_LOD,0,end_condition_expr_value->oid),
                    makecode(S_ILE),
                    makecode1(S_JMF, stat_list_code.size+6),
                    stat_list_code,
                    makecode2(S_LOD,offset,id_hash_node->oid),
                    makecode1(S_LDI,1),
                    makecode(S_IPLUS),
                    makecode2(S_STO,offset,id_hash_node->oid),
                    makecode1(S_JMP, -(stat_list_code.size+8)),
                    endcode()
                    );
    
    id_hash_node->class_node = id_class;
    return ok;
}

int sem_foreach_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux){
#if VERBOSE
    printf("@@ in sem_foreach_stat\n");
#endif
    Pnode id_node = root->child;
    Pnode expr_node = id_node->brother;
    Pnode stat_list_node = expr_node->brother;
    int ok = 1;
    int offset;
    Phash_node id_hash_node = find_visible_node(id_node->value.sval, f_loc_env, &offset);
    if (id_hash_node == NULL) {
        ok = 0;
        sprintf(error_msg, "Variable ID \"%s\" in FOREACH-STAT is not defined\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    ok = ok && (id_hash_node->class_node == CLVAR || id_hash_node->class_node == CLPAR);
    if (!ok) {
        sprintf(error_msg, "Variable ID \"%s\" in FOREACH-STAT must be a VAR or a PAR\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    
    Pschema expr_schema = new_schema_node(-1);
    Code expr_code = makecode(S_NOOP);
    ok = ok && sem_expr(expr_node, f_loc_env, &expr_schema, &expr_code, 0);
    ok = ok && (expr_schema->type == SCVECTOR);
    if (!ok) {
        sem_error(expr_node, "Type error: expr must be a VECTOR in FOR-EACH STAT\n");
    }
    ok = ok && are_compatible(expr_schema->p1, id_hash_node->schema);
    if (!ok) {
        sprintf(error_msg, "Variable ID \"%s\" must be of the same type of VECTOR elements in FOREACH STAT\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    
    char * id_aux_1;
    asprintf(&id_aux_1, "0_AUX_%d", f_loc_env->aux->last_oid);
    Phash_node expr_value = new_id_node(id_aux_1, CLCONST, f_loc_env->aux->last_oid);
    f_loc_env->aux->last_oid++;
    f_loc_env->aux->num_obj++;
    expr_value->schema = expr_schema;
    insert(expr_value, f_loc_env->aux->locenv);
    *code_new_aux = appcode(*code_new_aux, makecode1(S_NEWS, compute_size(expr_schema)));
    
    char * id_aux_2;
    asprintf(&id_aux_2, "0_AUX_%d", f_loc_env->aux->last_oid);
    Phash_node index = new_id_node(id_aux_2, CLCONST, f_loc_env->aux->last_oid);
    f_loc_env->aux->last_oid++;
    f_loc_env->aux->num_obj++;
    index->schema = new_schema_node(SCINT);
    insert(index, f_loc_env->aux->locenv);
    *code_new_aux = appcode(*code_new_aux, makecode1(S_NEW, sizeof(int)));
    
    Code e_s_il_code;
    if((expr_schema->p1)->type == SCSTRUCT || (expr_schema->p1)->type == SCVECTOR)
        e_s_il_code = makecode1(S_SIL, compute_size(expr_schema->p1));
    else
        e_s_il_code = makecode1(S_EIL, compute_size(expr_schema->p1));
    
    int not_used;
    Code stat_list_code = makecode(S_NOOP);
    ok = ok && sem_stat_list(stat_list_node, f_loc_env, &not_used, &stat_list_code, code_new_aux);
    
    *code = concode(*code,
                    makecode1(S_LDI,0),
                    makecode2(S_STO,0,index->oid),
                    expr_code,
                    makecode2(S_STO,0,expr_value->oid),
                    makecode2(S_LDA,0,expr_value->oid),
                    makecode2(S_LOD,0,index->oid),
                    makecode1(S_IXA,compute_size(expr_schema->p1)),
                    e_s_il_code,
                    makecode2(S_STO,offset,id_hash_node->oid),
                    stat_list_code,
                    makecode2(S_LOD,0,index->oid),
                    makecode1(S_LDI,1),
                    makecode(S_IPLUS),
                    makecode2(S_STO,0,index->oid),
                    makecode2(S_LOD,0,index->oid),
                    makecode1(S_LDI,expr_schema->size),
                    makecode(S_EQU),
                    makecode1(S_JMF, -(stat_list_code.size+12)),
                    endcode()
                    );
    return ok;
}

int sem_return_stat(Pnode root, Phash_node f_loc_env, Code * code){
#if VERBOSE
    printf("@@ in sem_return_stat ***\n");
#endif
    Pnode expr_node = root->child;
    
    Pschema expr_schema = new_schema_node(-1);
    int ok = sem_expr(expr_node, f_loc_env, &expr_schema, code, 0);
    ok = ok && are_compatible(expr_schema, f_loc_env->schema);
    if(!ok) {
        sem_error(expr_node, "Type error: RETURN-EXPR type must be the same as in function definition\n");
    }
    
    *code = appcode(*code, makecode(S_FAKE_RETURN));
    return ok;
}

int sem_read_stat(Pnode root, Phash_node f_loc_env, Code * code){
#if VERBOSE
    printf("@@ in sem_read_stat\n");
#endif
    Pnode spec = root->child;
    Pnode id_node = spec->brother;
    int is_null;
    Code spec_code = makecode(S_NOOP);
    int ok = sem_specifier_opt(spec, f_loc_env, &spec_code, &is_null);
    int offset;
    Phash_node id_hash_node = find_visible_node(id_node->value.sval, f_loc_env, &offset);
    if (id_hash_node == NULL) {
        ok = 0;
        sprintf(error_msg, "Variable ID \"%s\" in READ-STAT is not defined\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    ok = ok && (id_hash_node->class_node == CLVAR || id_hash_node->class_node == CLPAR);
    if (!ok) {
        sprintf(error_msg, "Variable ID \"%s\" in READ-STAT must be a VAR or a PAR\n",id_node->value.sval);
        sem_error(id_node, error_msg);
    }
    
    if (is_null) {
        *code = appcode(*code, makecode_xread(S_READ, offset, id_hash_node->oid,make_format(id_hash_node->schema)));
    }else{
        *code = appcode(*code, spec_code);
        *code = appcode(*code,makecode_xread(S_FREAD, offset, id_hash_node->oid, make_format(id_hash_node->schema)));
    }
    
    return ok;
}

int sem_specifier_opt(Pnode root, Phash_node f_loc_env, Code * code, int * is_null){ // NULL or STRING
#if VERBOSE
    printf("@@ in sem_specifier_opt\n");
#endif
    Pnode specifier = root->child;
    Pschema type_spec = new_schema_node(-1);
    int ok = 1;
    *is_null = (specifier == NULL);
    int spec_ok = *is_null;
    if (!spec_ok) {
        ok = sem_expr(specifier, f_loc_env, &type_spec, code, 0);
        spec_ok = (type_spec->type == SCSTRING);
    }
    
    if (!spec_ok) {
        sem_error(specifier, "Type error, specifier in wr/write/rd/read call must be a STRING or NULL\n");//to_do
    }
    return ok && spec_ok;
}

int sem_write_stat(Pnode root, Phash_node f_loc_env, Code * code){
#if VERBOSE
    printf("@@ in sem_write_stat\n");
#endif
    Pnode spec = root->child;
    int is_null;
    Code spec_code = makecode(S_NOOP);
    int ok = sem_specifier_opt(spec, f_loc_env, &spec_code, &is_null);
    
    Pschema expr_schema = new_schema_node(-1);
    ok = ok && sem_expr(spec->brother, f_loc_env, &expr_schema, code, 0);
    if (is_null) {
        *code = appcode(*code, makecode_str(S_WRITE, make_format(expr_schema)));
    }else{
        *code = appcode(*code, spec_code);
        *code = appcode(*code,makecode_str(S_FWRITE, make_format(expr_schema)));
    }
    return ok;
}

int sem_math_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_math_expr\n");
#endif
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	
	int expr1_ok = sem_expr(expr1, f_loc_env, &expr1_type, code, 0);
	if(expr1_type->type != SCINT && expr1_type->type != SCREAL){
		sprintf(error_msg,"Type error, expected INT | REAL instead %s \n", "to_do");
		sem_error(expr1, error_msg);
	}
	int expr2_ok = sem_expr(expr2, f_loc_env, &expr2_type, code, 0);
	if(expr2_type->type != expr1_type->type){
		sprintf(error_msg,"Type mismatch, expected %s instead %s\n", "to_do", "to_do");
		sem_error(expr2, error_msg);
	}
	(*stype)->type = expr1_type->type;
    
    if (expr1_type->type == SCINT) {
        switch (root->qualifier) {
            case '+':
                *code = appcode(*code, makecode(S_IPLUS));
                break;
            case '-':
                *code = appcode(*code, makecode(S_IMINUS));
                break;
            case '*':
                *code = appcode(*code, makecode(S_ITIMES));
                break;
            case '/':
                *code = appcode(*code, makecode(S_IDIV));
                break;
            default:
                break;
        }
    }
    else{ //expr1_type->type == REAL
        switch (root->qualifier) {
            case '+':
                *code = appcode(*code, makecode(S_RPLUS));
                break;
            case '-':
                *code = appcode(*code, makecode(S_RMINUS));
                break;
            case '*':
                *code = appcode(*code, makecode(S_RTIMES));
                break;
            case '/':
                *code = appcode(*code, makecode(S_RDIV));
                break;
            default:
                break;
        }
        
    }
    
	return expr1_ok && expr2_ok;
}

int sem_logic_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_logic_expr\n");
#endif
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
    
    Code expr1_code = makecode(S_NOOP);
    Code expr2_code = makecode(S_NOOP);
	int expr1_ok = sem_expr(expr1, f_loc_env, &expr1_type, &expr1_code, 0);
	if(expr1_type->type != SCBOOL)
		sem_error(expr1, "Type error, expected BOOL in LOGIC-EXPR\n");
    
	int expr2_ok = sem_expr(expr2, f_loc_env, &expr2_type, &expr2_code, 0);
	if(expr2_type->type != SCBOOL)
		sem_error(expr2, "Type error, expected BOOL in LOGIC-EXPR\n");
	
    (*stype)->type = SCBOOL;
    
    if (root->qualifier == AND) {
        *code = concode(*code,
                        expr1_code,
                        makecode1(S_JMF, expr2_code.size+2),
                        expr2_code,
                        makecode1(S_JMP, 2),
                        make_ldc('0'),
                        endcode());
    }else{//root->qualifier == AND
        *code = concode(*code,
                        expr1_code,
                        makecode1(S_JMF, 3),
                        make_ldc('1'),
                        makecode1(S_JMP, expr2_code.size+1),
                        expr2_code,
                        endcode());
    }
	return expr1_ok && expr2_ok;
}

int sem_rel_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_rel_expr\n");
#endif
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	int expr1_ok = sem_expr(expr1, f_loc_env, &expr1_type, code, 0);
	int expr2_ok = sem_expr(expr2, f_loc_env, &expr2_type, code, 0);
    
	int type_ok = 1;
	switch(root->qualifier){
		case EQ:
		case NE:
			type_ok = (expr1_type->type == expr2_type->type);
            if(!type_ok)
                sem_error(root, "Type mismatch in relational expression\n");//to_do
			break;
		case '>':
		case GE:
		case '<':
		case LE:
            type_ok = (expr1_type->type == SCINT || expr1_type->type == SCCHAR || expr1_type->type == SCREAL || expr1_type->type == SCSTRING);
            if (!type_ok) {
                sem_error(root, "Type error in relational expression, expected INT, CHAR, REAL or STRING\n");//to_do
            }
            type_ok = type_ok && (expr1_type->type == expr2_type->type);
            if(!type_ok)
                sem_error(root, "Type mismatch in relational expression\n");//to_do
			break;
		case IN:
            if (expr2_type->type == SCVECTOR) {
                type_ok = are_compatible(expr1_type, expr2_type->p1 );
            }
            if(!type_ok)
                sem_error(root, "Type error using IN relational expression\n");//to_do
			break;
		default:
            type_ok = 0;
			sem_error(root, "Some weird qualification in relational expression\n");
	}
    
	(*stype)->type = SCBOOL;
    
    switch (root->qualifier) {
        case EQ:
            *code = appcode(*code, makecode(S_EQU));
            break;
        case NE:
            *code = appcode(*code, makecode(S_NEQ));
            break;
        case IN:
            *code = appcode(*code, makecode(S_IN));
            break;
        case '<':
            switch(expr1_type->type){
                case SCCHAR:
                    *code = appcode(*code, makecode(S_CLT));
                    break;
                case SCINT:
                    *code = appcode(*code, makecode(S_ILT));
                    break;
                case SCREAL:
                    *code = appcode(*code, makecode(S_RLT));
                    break;
                case SCSTRING:
                    *code = appcode(*code, makecode(S_SLT));
                    break;
                default:
                    sem_error(root, "Some weird qualification in < expression\n");
            }
            break;
        case '>':
            switch(expr1_type->type){
                case SCCHAR:
                    *code = appcode(*code, makecode(S_CGT));
                    break;
                case SCINT:
                    *code = appcode(*code, makecode(S_IGT));
                    break;
                case SCREAL:
                    *code = appcode(*code, makecode(S_RGT));
                    break;
                case SCSTRING:
                    *code = appcode(*code, makecode(S_SGT));
                    break;
                default:
                    sem_error(root, "Some weird qualification in > expression\n");
            }
            break;
        case LE:
            switch(expr1_type->type){
                case SCCHAR:
                    *code = appcode(*code, makecode(S_CLE));
                    break;
                case SCINT:
                    *code = appcode(*code, makecode(S_ILE));
                    break;
                case SCREAL:
                    *code = appcode(*code, makecode(S_RLE));
                    break;
                case SCSTRING:
                    *code = appcode(*code, makecode(S_SLE));
                    break;
                default:
                    sem_error(root, "Some weird qualification in <= expression\n");
            }
            break;
            
            break;
        case GE:
            switch(expr1_type->type){
                case SCCHAR:
                    *code = appcode(*code, makecode(S_CGE));
                    break;
                case SCINT:
                    *code = appcode(*code, makecode(S_IGE));
                    break;
                case SCREAL:
                    *code = appcode(*code, makecode(S_RGE));
                    break;
                case SCSTRING:
                    *code = appcode(*code, makecode(S_SGE));
                    break;
                default:
                    sem_error(root, "Some weird qualification in >= expression\n");
            }
            break;
        default:
            break;
    }
	return expr1_ok && expr2_ok && type_ok;
}

int sem_neg_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_neg_expr\n");
#endif
	Pschema expr_type = new_schema_node(-1);
	int expr_ok = sem_expr(root->child, f_loc_env, &expr_type, code, 0);
	switch(root->qualifier){
		case '-':
            print_sch(expr_type);
			if(expr_type->type != SCINT && expr_type->type != SCREAL){
				sprintf(error_msg,"Type error, expected INT | REAL\n");
				sem_error(root->child, error_msg);
			}
			(*stype)->type = expr_type->type;
            
            if (expr_type->type == SCINT) {
                *code = appcode(*code, makecode(S_IUMI));
            }else{ //expr_type->type == REAL
                *code = appcode(*code, makecode(S_RUMI));
            }
            break;
		case NOT:
			if(expr_type->type != SCBOOL){
				sprintf(error_msg,"Type error, expected BOOL in NEG-EXPR\n");
				sem_error(root->child, error_msg);
			}
			(*stype)->type = SCBOOL;
            
            *code = appcode(*code, makecode(S_NEG));
            break;
	}
	return expr_ok;
}

int sem_wr_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_wr_expr\n");
#endif
    int is_null;
    Code spec_code = makecode(S_NOOP);
    int ok = sem_specifier_opt(root->child, f_loc_env, &spec_code, &is_null);
    int expr_ok = 1;
    if (ok) {
        expr_ok = sem_expr(root->child->brother, f_loc_env, stype, code, 0);
    }
    if (is_null) {
        *code = appcode(*code, makecode_str(S_WR, make_format(*stype)));
    }else{
        *code = appcode(*code, spec_code);
        *code = appcode(*code,makecode_str(S_FWR, make_format(*stype)));
    }
    
    return ok && expr_ok;
}

int sem_rd_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_rd_expr\n");
#endif
    int is_null;
    Code spec_code = makecode(S_NOOP);
    int ok = sem_specifier_opt(root->child, f_loc_env, &spec_code, &is_null);
    printf("\n## dopo specifier_opt\n");
    int dom_ok = 1;
    if (ok) {
        printf("\n## xpecifier_opt è ok \n");
        dom_ok = sem_domain(root->child->brother, f_loc_env, stype, code);
    }
    if (is_null) {
        *code = appcode(*code, makecode_str(S_RD, make_format(*stype)));
    }else{
        *code = appcode(*code, spec_code);
        *code = appcode(*code,makecode_str(S_FRD, make_format(*stype)));
    }
    return ok && dom_ok;
}

int sem_instance_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_instance_expr\n");
#endif
	int expr_ok = 0;
	int count = 0;
	Pschema current_schema = new_schema_node(-1); //allocate schema
	Pnode current_node;
	switch(root->qualifier){
		case STRUCT:
			(*stype)->type = SCSTRUCT;
			
			current_node = root->child; //first element of struct
			
			expr_ok = sem_expr(current_node, f_loc_env, &current_schema, code, 0); //eval first child's schema
			(*stype)->p1 = current_schema; // attach to root's schema the schema of first child
			current_node = current_node->brother; //switch to the firse brother
            
            int num_field=1;
            
			while (current_node){//cicle for the other brother
				Pschema next = new_schema_node(-1);
				expr_ok = sem_expr(current_node, f_loc_env, &next, code, 0);
                //current_schema->id = NULL;
				current_schema->p2 = next;
				current_schema = next;
				current_node = current_node->brother;
                num_field++;
			}
            
            *code = appcode(*code, makecode2(S_CAT, num_field, compute_size(*stype)));
            
            break;
		case VECTOR:
			(*stype)->type = SCVECTOR;
			
			current_node = root->child;
			
			expr_ok = sem_expr(current_node, f_loc_env, &current_schema, code, 0);
			current_node = current_node->brother;
			count++;
			while (current_node){
				Pschema next = new_schema_node(-1);
				expr_ok = expr_ok && sem_expr(current_node, f_loc_env, &next, code, 0);
				if(!are_compatible(next,current_schema)){
					sem_error(current_node, "Type Error, Vector type elements must be uniform\n");
					break;
				}
				current_node = current_node->brother;
				count++;
			}
			(*stype)->size = count;
			(*stype)->p1 = current_schema;
            
            *code = appcode(*code, makecode2(S_CAT, (*stype)->size, compute_size(*stype)));
            break;
	}
	return expr_ok;
}

int sem_func_call(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_func_call\n");
#endif
    Pnode id_node = root->child;
    Pnode param_node = id_node->brother;
    int id_ok, expr_ok = 1, param_ok = 1;
    int offset;
    Phash_node h_id_node = find_visible_node(id_node->value.sval, f_loc_env, &offset);
    id_ok = (h_id_node != NULL) && (h_id_node->class_node == CLFUNC);
    if (!id_ok) {
        sprintf(error_msg, "\"%s\" is not a visible FUNC name\n", id_node->value.sval);
        sem_error(id_node, error_msg);
    }
#if VERBOSE
    print_func_node(h_id_node);
#endif
    Formal * current_formal = (h_id_node->aux)->formal;
    Pschema current_schema;
    
    while (param_node != NULL && current_formal != NULL) {
        current_schema = new_schema_node(-1);
        
        expr_ok = expr_ok && sem_expr(param_node, f_loc_env, &current_schema, code, 0);
        
        print_sch(current_schema);
        print_sch(current_formal->formal->schema);
        param_ok = are_compatible(current_schema, current_formal->formal->schema);
        if(!param_ok)
            break;
        
        param_node = param_node->brother;
        current_formal = current_formal->next;
    }
    param_ok = param_ok && (param_node == NULL && current_formal == NULL);
    if (!param_ok) {
        sem_error(id_node, "Formal parameter must be compatible with actual parameter in function call\n");
    }
    (*stype) = h_id_node->schema;
    
    *code = concode(*code,
                    make_push_pop(h_id_node->aux->formals_num, h_id_node->aux->num_obj, offset , h_id_node->oid),
                    endcode());
    return id_ok && expr_ok && param_ok;
}

int sem_cond_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_cond_expr\n");
#endif
    
	Pnode main_expr_node = root->child;
	Pnode first_expr_node = main_expr_node->brother;
	Pnode elsif_expr_node = first_expr_node->brother;
	Pnode else_expr_node = elsif_expr_node->brother;
    
	//check constraint on conditional clause
	Pschema main_expr_type = new_schema_node(-1);
	int main_expr_ok = sem_expr(main_expr_node, f_loc_env, &main_expr_type, code, 0);
    
	if (main_expr_type->type!=SCBOOL){
		sem_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}
    
	//check constraint on first and last alternative
	Pschema * first_expr_type = stype;
    Code first_expr_code = makecode(S_NOOP);
	int first_expr_ok = sem_expr(first_expr_node, f_loc_env,first_expr_type, &first_expr_code, 0);
    
	Pschema else_expr_type = new_schema_node(-1);
    Code else_expr_code = makecode(S_NOOP);
	int else_expr_ok = sem_expr(else_expr_node, f_loc_env, &else_expr_type, &else_expr_code, 0);
    
	if (!are_compatible(*first_expr_type, else_expr_type)){
		sem_error(else_expr_node, "Type error, alternatives are of different type\n");
	}
    
    int offset_to_exit = else_expr_code.size+1;
	//check constraint on elsif part
	Pschema elsif_expr_type = new_schema_node(-1);
    Code elsif_expr_code = makecode(S_NOOP);
	int elsif_expr_ok = sem_elsif_expr_list_opt(elsif_expr_node, f_loc_env, &elsif_expr_type, &elsif_expr_code, &offset_to_exit);
	
	if (elsif_expr_type != NULL && !are_compatible(*first_expr_type, elsif_expr_type)){
		sem_error(elsif_expr_node, "Type error, alternatives are of different type\n");
	}
    printf("elsif_expr_code\n");
    print_code(stdout, &elsif_expr_code);
    printf("elsif_expr_code\n");
    
    *code = concode(*code,
                    makecode1(S_JMF, first_expr_code.size+2),
                    first_expr_code,
                    elsif_expr_code,
                    makecode1(S_JMP, else_expr_code.size+1),
                    else_expr_code,
                    endcode());
    
    
	return main_expr_ok && first_expr_ok && elsif_expr_ok && else_expr_ok;
}

int sem_elsif_expr_list_opt(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int * offset_to_exit){
#if VERBOSE
    printf("@@ in sem_elsif_expr_list_opt\n");
#endif
	if (root->child == NULL){
		*stype = NULL;
		return 1;
	}
    int first = 1;
    
	Pnode main_expr_node = root->child;
    Pnode expr_node;
    Pschema * expr_type = NULL;
    int main_expr_ok = 0, expr_ok = 0;
    
    Stack_node_code * top = NULL;
    
    Code * ptemp_code;
    
    while (main_expr_node) {
        expr_node = main_expr_node->brother;
        
        ptemp_code = (Code *)malloc(sizeof(Code));
        *ptemp_code = makecode(S_NOOP);
        
        //check contraint on conditional clause
        Pschema main_expr_type = new_schema_node(-1);
        main_expr_ok = sem_expr(main_expr_node, f_loc_env, &main_expr_type, ptemp_code, 0);
        
        if (main_expr_type->type!=SCBOOL){
            sem_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
        }
        
        Code expr_code;
        //check constraint on first and other elsif alternative
        if(first){
            expr_type = stype;
            expr_code = makecode(S_NOOP);
            expr_ok = sem_expr(expr_node, f_loc_env, expr_type, &expr_code, 0);
            first = 0;
        }else{
            Pschema elsif_expr_type = new_schema_node(-1);
            expr_code = makecode(S_NOOP);
            expr_ok = sem_expr(expr_node, f_loc_env, &elsif_expr_type, &expr_code, 0);
            
            if (!are_compatible(*expr_type, elsif_expr_type)){
                sem_error(expr_node, "Type error, alternatives are of different type\n");
            }
            
        }
        
        *ptemp_code = concode(*ptemp_code,
                              makecode1(S_JMF, expr_code.size+1+1),
                              expr_code,
                              endcode());
        
        StackPush(&top, ptemp_code);
        printf("top = %p\n",top);
        
        main_expr_node = expr_node->brother;
    }
    
    Code reverse = makecode(S_NOOP);
    while (top != NULL) {
        printf("top != NULL......\n");
        Code * current_code = StackPop(&top);
        (*offset_to_exit) += (current_code->size+1);
        reverse = concode(makecode1(S_JMP, *offset_to_exit),
                          *current_code,
                          reverse,
                          endcode());
    }
    
    *code = appcode(*code, reverse);
	return main_expr_ok && expr_ok;
}

int sem_built_in_call(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code){
#if VERBOSE
    printf("@@ in sem_built_in_call\n");
#endif
	Pschema built_in_call_type = new_schema_node(-1);
	int built_in_call_ok = sem_expr(root->child, f_loc_env, &built_in_call_type, code, 0);
    
	switch(root->qualifier){
		case TOINT:
			if(built_in_call_type->type != SCREAL){
				sem_error(root->child, "Type error, expected REAL\n");
			}
			(*stype)->type = SCINT;
            *code = appcode(*code, makecode(S_TOINT));
            break;
		case TOREAL:
			if(built_in_call_type->type != SCINT){
				sem_error(root->child, "Type error, expected INT\n");
			}
			(*stype)->type = SCREAL;
            *code = appcode(*code, makecode(S_TOREAL));
            break;
	}
	return built_in_call_ok;
}

int sem_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int is_addr){
#if VERBOSE
    printf("@@ in sem_expr\n");
    treeprint(root, " ");
    printf(".\n");
#endif
	int expr_ok = 1;
    Class not_used;
    int is_s = 0;
    
	switch(root->type){
		case T_CHARCONST:
            printf("expr is char\n");
			(*stype)->type = SCCHAR;
            *code = appcode(*code, make_ldc(root->value.cval));
			break;
		case T_INTCONST:
			(*stype)->type = SCINT;
            *code =appcode(*code, make_ldi(root->value.ival));
			break;
		case T_REALCONST:
			(*stype)->type = SCREAL;
            *code =appcode(*code, make_ldr(root->value.rval));
			break;
		case T_STRCONST:
			(*stype)->type = SCSTRING;
            *code =appcode(*code, make_lds(root->value.sval));
			break;
		case T_BOOLCONST:
            printf("expr is bool\n");
			(*stype)->type = SCBOOL;
            *code = appcode(*code, make_ldc(root->value.cval));
			break;
		case T_NONTERMINAL:
            switch(root->value.ival){
                case NLEFT_HAND_SIDE:
                    expr_ok = sem_left_hand_side(root, f_loc_env, stype, &not_used, code, is_addr, &is_s);
                    break;
                case NMATH_EXPR:
                    expr_ok = sem_math_expr(root, f_loc_env, stype, code);
                    break;
                case NLOGIC_EXPR:
                    expr_ok = sem_logic_expr(root, f_loc_env, stype, code);
                    break;
                case NREL_EXPR:
                    expr_ok = sem_rel_expr(root, f_loc_env, stype, code);
                    break;
                case NNEG_EXPR:
                    expr_ok = sem_neg_expr(root, f_loc_env, stype, code);
                    break;
                case NWR_EXPR:
                    expr_ok = sem_wr_expr(root, f_loc_env, stype, code);
                    break;
                case NRD_EXPR:
                    expr_ok = sem_rd_expr(root, f_loc_env, stype, code);
                    break;
                case NINSTANCE_EXPR:
                    expr_ok = sem_instance_expr(root, f_loc_env, stype, code);
                    break;
                case NFUNC_CALL:
                    expr_ok = sem_func_call(root, f_loc_env, stype, code);
                    break;
                case NCOND_EXPR:
                    expr_ok = sem_cond_expr(root, f_loc_env, stype, code);
                    break;
                case NBUILT_IN_CALL:
                    expr_ok = sem_built_in_call(root, f_loc_env, stype, code);
                    break;
                default:
                    sem_error(root, "Some weird nonterminal node in expr\n");
            }
            break;
		default:
            printf("%d\n\n", root->type);
			sem_error(root, "Some weird terminal node in expr\n");
	}
	return expr_ok;
}

void sem_error(Pnode node, char * msg){
    if (node == NULL)
        fprintf(stderr, "[NODE IS NULL] Semantic error: %s", msg);
    else
        fprintf(stderr, "Row: %d Semantic error: %s", node->row, msg);
    exit(EXIT_FAILURE);
}

// this method scan every time all the code previusly generated, we can make it more smart
void cleanup_return(Stat * start, int code_len,Code * code){
    Stat * stat = start;
    int i = 0;
    if((code->tail)->op==S_FAKE_RETURN){
        (code->tail)->op=S_RETURN;
        (code->tail)->args[0].ival=0;//useless
    }
    else
        *code = appcode(*code,makecode(S_RETURN));
    
    while(stat){
        if(stat->op==S_FAKE_RETURN){
            stat->op=S_JMP;
            stat->args[0].ival=code->size-code_len-i;
        }
        i++;
        stat = stat->next;
    }
    
}

void cleanup_goto(Code * code, Poid2address * func_table){
    Stat * stat = code->head;
    while(stat){
        if(stat->op==S_FAKE_GOTO){
            stat->op=S_GOTO;
            stat->args[0].ival=get_f_addr_by_oid(stat->args[0].ival, func_table);
        }
        stat = stat->next;
    }
    
}