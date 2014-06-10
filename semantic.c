#include "semantic.h"

char error_msg[100];


int program(Pnode root, Phash_node f_loc_env, int not_first){
	return func_decl(root->child, f_loc_env, not_first);
}
int func_decl(Pnode root, Phash_node f_loc_env, int not_first){
	Pnode id = root->child;
	Pnode current = id->brother;
    Pschema stype;
    
    Phash_node new_f_loc_env;
    
    if (not_first)
        new_f_loc_env = getNode(id->value.sval, f_loc_env->locenv);
    else
        new_f_loc_env = f_loc_env;
    
    
	int decl_list_opt_ok = decl_list_opt(current, new_f_loc_env);
	current = current->brother;
	int domain_ok = domain(current, new_f_loc_env, stype);
	current = current->brother;
	int type_sect_opt_ok = type_sect_opt(current, new_f_loc_env);
	current = current->brother;
	int var_sect_opt_ok = var_sect_opt(current, new_f_loc_env);
	current = current->brother;
	int const_sect_opt_ok = const_sect_opt(current, new_f_loc_env);
	current = current->brother;
	int func_list_opt_ok = func_list_opt(current, new_f_loc_env);
	current = current->brother;
	int func_body_ok = func_body(current, new_f_loc_env);
    //check id
    
}
int decl_list_opt(Pnode root, Phash_node f_loc_env){
	Pnode current = root->child;
	int decl_list_opt_ok = 1;
	while(current != NULL){
		decl_list_opt_ok = decl_list_opt_ok && decl(current, f_loc_env);
		current = current->brother;
	}
	return decl_list_opt_ok;
}
int decl(Pnode root, Phash_node f_loc_env){
    
}
int id_list(Pnode root, Phash_node f_loc_env){
    
}
int domain(Pnode root, Phash_node f_loc_env, Pschema stype){
    
}
int struct_domain(Pnode root, Phash_node f_loc_env){
    
}
int vector_domain(Pnode root, Phash_node f_loc_env){
    
}
int type_sect_opt(Pnode root, Phash_node f_loc_env){
    
}
int var_sect_opt(Pnode root, Phash_node f_loc_env){
    
}
int const_sect_opt(Pnode root, Phash_node f_loc_env){
    
}
int func_list_opt(Pnode root, Phash_node f_loc_env){
    int ok = 1;
    Pnode func_decl_node = root->child;
    if (func_decl_node == NULL) {
        return ok;
    }
    while (func_decl_node != NULL) {
        ok = ok && func_decl(func_decl_node, f_loc_env, 1);
        func_decl_node = func_decl_node->brother;
    }
    return ok;
}
int func_body(Pnode root, Phash_node f_loc_env){
    Pnode id1 = root->child;
    Pnode stat_list_node = id1->brother;
    Pnode id2 = stat_list_node->brother;
    int ok = 1;
    
    ok = (strcmp(f_loc_env->name, id1->value.sval) == 0);
    if (!ok) {
        semantic_error(id1, "Function ID different from ID in function body BEGIN\n");
    }
    ok = ok && stat_list(stat_list_node, f_loc_env);
    ok = ok && (strcmp(id1->value.sval, id2->value.sval) == 0);
    if (!ok) {
        semantic_error(id2, "Function ID different from ID in function body END\n");
    }
    return ok;
}
int stat_list(Pnode root, Phash_node f_loc_env){
    Pnode stat_node = root->child;
    int ok = 1;
    while (stat_node != NULL) {
        ok = ok && stat(stat_node, f_loc_env);
        stat_node = stat_node->brother;
    }
    return ok;
}
int stat(Pnode root, Phash_node f_loc_env){
    int ok;
    Pnode child = root->child;
    switch (child->value.ival) {//always a NONTERMINAL
        case NASSIGN_STAT:
            ok = assign_stat(child, f_loc_env);
            break;
        case NIF_STAT:
            ok = if_stat(child, f_loc_env);
            break;
        case NWHILE_STAT:
            ok = while_stat(child, f_loc_env);
            break;
        case NFOR_STAT:
            ok = for_stat(child, f_loc_env);
            break;
        case NFOREACH_STAT:
            ok = foreach_stat(child, f_loc_env);
            break;
        case NRETURN_STAT:
            ok = return_stat(child, f_loc_env);
            break;
        case NREAD_STAT:
            ok = read_stat(child, f_loc_env);
            break;
        case NWRITE_STAT:
            ok = write_stat(child, f_loc_env);
            break;
        default:
            semantic_error(child, "Some weird nonterminal node in lhs\n");
            break;
    }
    return ok;
}
int assign_stat(Pnode root, Phash_node f_loc_env){
    Pnode lhs_node = root->child;
    Pnode expr_node = lhs_node->brother;
    int ok = 1;
    
    Pschema lhs_schema;
    Class lhs_class; //maybe is not allocated...
    Pschema expr_schema = new_schema_node(-1);
    
    ok = ok && left_hand_side(lhs_node, f_loc_env, lhs_schema, &lhs_class);
    ok = ok && (lhs_class == CLVAR || lhs_class == CLPAR); //not a CONST
    if (!ok) {
        semantic_error(root, "Semantic error, cannot assign value to a CONST\n");//to_do
    }
    
    ok = ok && expr(expr_node, f_loc_env, expr_schema);
    
    ok = ok && are_compatible(lhs_schema, expr_schema);
    if (!ok) {
        semantic_error(root, "Type error in ASSIGNMENT, type must be compatible\n");//to_do
    }
    return ok;
}
int left_hand_side(Pnode root, Phash_node f_loc_env, Pschema stype, Class * lhs_class){//to be called on a SCHEMA not mallocated
    Phash_node h_node;
    int lhs_ok;
    Pnode child = root->child;
    switch (child->type) {
        case T_ID:
            h_node = find_visible_node(child->value.sval, f_loc_env);
            if (h_node == NULL) {
                lhs_ok = 0;
                semantic_error(child, "Use of not visible ID\n");
                //semantic_error("Use of not visible ID %s\n", root->value.sval);
            }
            lhs_ok = (h_node->class_node == CLVAR || h_node->class_node == CLPAR || h_node->class_node == CLCONST);
            if (!lhs_ok) {
                semantic_error(child, "An lhs cannot be a function or a constant name\n");
            }
            *lhs_class = h_node->class_node;
            stype = h_node->schema; //TODO check about malloc...
            break;
        case T_NONTERMINAL:
            switch (child->value.ival) {
                case NFIELDING:
                    lhs_ok = fielding(child, f_loc_env, stype, lhs_class);//TODO
                    break;
                case NINDEXING:
                    lhs_ok = indexing(child, f_loc_env, stype, lhs_class);//TODO
                    break;
                default:
                    semantic_error(child, "Some weird nonterminal node in lhs\n");
                    break;
            }
            break;
        default:
            semantic_error(child, "Some weird terminal node in lhs\n");
            break;
    }
}
int fielding(Pnode root, Phash_node f_loc_env, Pschema stype, Class * lhs_class){
    int ok_field;
    Pnode lhs_node = root->child;
    // Pschema lhs_type = new_schema_node(-1);
    Pschema lhs_type = stype;
    
    ok_field = left_hand_side(lhs_node, f_loc_env, lhs_type, lhs_class);
    ok_field = ok_field && (lhs_type->type == STRUCT);
    if (!ok_field) {
        semantic_error(root, "Type error, cannot use . on a lhs that is not a STRUCT");//to_do
    }
    //lhs is a STRUCT
    Pnode id_node = lhs_node->brother;
    //lhs.id must exist... so check lhs children
    Pschema lhs_attr = lhs_type->p1;
    int found = 0;
    while (found==0 && lhs_attr != NULL) {
        if (strcmp(id_node->value.sval, lhs_attr->id)==0) {
            stype = lhs_attr->p1;
            found = 1;
        }
        lhs_attr = lhs_attr->p2;
    }
    ok_field = ok_field && found;
    if (!found) {
        semantic_error(root, "Semantic error, trying to access a non-existent field");//to_do
    }
    return ok_field;
}
int indexing(Pnode root, Phash_node f_loc_env, Pschema stype, Class * lhs_class){
    int ok_index;
    Pnode lhs_node = root->child;
    Pnode index_node = lhs_node->brother;
    
    Pschema lhs_type = stype;
    ok_index = left_hand_side(lhs_node, f_loc_env, lhs_type, lhs_class);
    ok_index = ok_index && (lhs_type->type == VECTOR);
    if (!ok_index) {
        semantic_error(root, "Semantic error, cannot index a non-VECTOR");//to_do
    }
    
    Pschema index_type = new_schema_node(-1);
    ok_index = ok_index && expr(index_node, f_loc_env, index_type);
    ok_index = ok_index && (index_type->type == INT);
    if (!ok_index) {
        semantic_error(root, "Semantic error, index must be of type INT");//to_do
    }
    
    stype = lhs_type->p1;
    return ok_index;
}
int if_stat(Pnode root, Phash_node f_loc_env){
    
	Pnode main_expr_node = root->child;
	Pnode if_stat_list_node = main_expr_node->brother;
	Pnode elsif_stat_list_opt_node = if_stat_list_node->brother;
	Pnode else_stat_list_node = elsif_stat_list_opt_node->brother;

	//check contraint on conditional clause
	Pschema main_expr_type = new_schema_node(-1);;
	int main_expr_ok = expr(main_expr_node, f_loc_env, main_expr_type);

	if (main_expr_type->type!=BOOL){
		semantic_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}

	int if_stat_list_ok = stat_list(if_stat_list_node, f_loc_env);

	int else_stat_list_ok = stat_list(else_stat_list_node, f_loc_env);

	int elsif_stat_list_opt_ok = elsif_stat_list_opt(elsif_stat_list_opt_node, f_loc_env);

	return main_expr_ok && if_stat_list_ok && elsif_stat_list_opt_ok && else_stat_list_ok;
}
int elsif_stat_list_opt(Pnode root, Phash_node f_loc_env){
    if (root->child == NULL){
		return 1;
	}

	Pnode main_expr_node = root->child;
	Pnode stat_list_node = main_expr_node->brother;
	Pnode elsif_stat_list_opt_node = stat_list_node->brother;

	//check contraint on conditional clause
    Pschema main_expr_type = new_schema_node(-1);;
    int main_expr_ok = expr(main_expr_node, f_loc_env, main_expr_type);

    if (main_expr_type->type!=BOOL){
		semantic_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}

    int stat_list_ok = stat_list(stat_list_node, f_loc_env);

    int elsif_stat_list_opt_ok = elsif_stat_list_opt(elsif_stat_list_opt_node, f_loc_env);
	
	return main_expr_ok && stat_list_ok && elsif_stat_list_opt_ok;
}
int while_stat(Pnode root, Phash_node f_loc_env){
    
}
int for_stat(Pnode root, Phash_node f_loc_env){
    
}
int foreach_stat(Pnode root, Phash_node f_loc_env){
    
}
int return_stat(Pnode root, Phash_node f_loc_env){
    
}
int read_stat(Pnode root, Phash_node f_loc_env){
    
}
int specifier_opt(Pnode specifier_opt, Phash_node f_loc_env){ // NULL or STRING
    Pnode specifier = specifier_opt->child;
    Pschema type_spec = new_schema_node(-1);
    int ok;
    int spec_ok = (specifier == NULL);
    if (!spec_ok) {
        ok = expr(specifier->child, f_loc_env, type_spec);
        spec_ok = (type_spec->type == STRING);
    }
    if (!spec_ok) {
        semantic_error(specifier, "Type error, specifier in wr/write/rd/read call must be a STRING or NULL");//to_do
    }
    return ok && spec_ok;
}

int write_stat(Pnode root, Phash_node f_loc_env){
    
}
int math_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	
	int expr1_ok = expr(expr1, f_loc_env, expr1_type);
	if(expr1_type->type != INT || expr1_type->type != REAL){
		//sprintf(error_msg,"Type error, expected INT | REAL instead %s \n", tabsem_types[expr1_type]);
		sprintf(error_msg,"Type error, expected INT | REAL instead %s \n", "to_do");
		semantic_error(expr1, error_msg);
	}
	int expr2_ok = expr(expr2, f_loc_env, expr2_type);
	if(expr2_type->type != expr1_type->type){
		//sprintf(error_msg,"Type mismatch, expected %s instead %s\n", tabsem_types[expr1_type],tabsem_types[expr2_type]);
		sprintf(error_msg,"Type mismatch, expected %s instead %s\n", "to_do", "to_do");
		semantic_error(expr2, error_msg);
	}
	stype->type = expr1_type->type;
	return expr1_ok && expr2_ok;
}
int logic_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
    
	int expr1_ok = expr(expr1, f_loc_env, expr1_type);
	if(expr1_type->type != BOOL)
		semantic_error(expr1, "Type error, expected BOOL\n");
	int expr2_ok = expr(expr2, f_loc_env, expr2_type);
	if(expr2_type->type != BOOL)
		semantic_error(expr2, "Type error, expected BOOL\n");
	stype->type = BOOL;
	return expr1_ok && expr2_ok;
}
int rel_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	int expr1_ok = expr(expr1, f_loc_env, expr1_type);
	int expr2_ok = expr(expr2, f_loc_env, expr2_type);
	int type_ok;
	switch(root->qualifier){
		case EQ:
		case NE:
			type_ok = expr1->type == expr2->type;
            if(!type_ok)
                semantic_error(root, "Type mismatch in relational expression\n");//to_do
			break;
		case '>':
		case GE:
		case '<':
		case LE:
            type_ok = (expr1_type->type == INT || expr1_type->type == CHAR || expr1_type->type == REAL || expr1_type->type == STRING);
            if (!type_ok) {
                semantic_error(root, "Type error in relational expression, expected INT, CHAR, REAL or STRING\n");//to_do
            }
            type_ok = type_ok && (expr1_type->type == expr2_type->type);
            if(!type_ok)
                semantic_error(root, "Type mismatch in relational expression\n");//to_do
			break;
		case IN:
            if (expr2_type->type == VECTOR) {
                type_ok = are_compatible(expr1_type, expr2_type->p1 );
            }
            if(!type_ok)
                semantic_error(root, "Type error using IN relational expression\n");//to_do
			break;
		default:
			semantic_error(root, "Some weird qualification in relational expression\n");
	}
    
	stype->type = BOOL;
	return expr1_ok && expr2_ok && type_ok;
}
int neg_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	Pschema expr_type = new_schema_node(-1);
	int expr_ok = expr(root->child, f_loc_env, expr_type);
	switch(root->qualifier){
		case '-':
			if(expr_type->type != INT || expr_type->type != REAL){
				//sprintf(error_msg,"Type error, expected INT | REAL instead of %s \n", tabsem_types[expr_type]);//
				sprintf(error_msg,"Type error, expected INT | REAL");
				semantic_error(root->child, error_msg);
			}
			stype->type = expr_type->type;
            break;
		case NOT:
			if(expr_type->type != BOOL){
				//sprintf(error_msg,"Type error, expected BOOL instead of %s \n", tabsem_types[expr_type]);//
				sprintf(error_msg,"Type error, expected BOOL");
				semantic_error(root->child, error_msg);
			}
			stype->type = BOOL;
            break;
	}
	return expr_ok;
}
int wr_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
    int ok = specifier_opt(root->child, f_loc_env);
    int expr_ok;
    if (ok) {
        expr_ok = expr(root->child->brother, f_loc_env, stype);
    }
    return ok && expr_ok;
}
int rd_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
    int ok = specifier_opt(root->child, f_loc_env);
    int dom_ok;
    if (ok) {
        dom_ok = domain(root->child->brother, f_loc_env, stype);
    }
    return ok && dom_ok;
}
int instance_expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	int expr_ok;
	int count = 0;
	Pschema current_schema = new_schema_node(-1); //allocate schema
	Pnode current_node;
	switch(root->qualifier){
		case STRUCT:
			stype->type = STRUCT;
			
			current_node = root->child; //first element of struct
			
			expr_ok = expr(current_node, f_loc_env, current_schema); //eval first child's schema
			stype->p1 = current_schema; // attach to root's schema the schema of first child
			current_node = current_node->brother; //switch to the firse brother
            
			while (current_node){//cicle for the other brother
				Pschema next = new_schema_node(-1);
				expr_ok = expr(current_node, f_loc_env, next);
				current_schema->p2 = next;
				current_schema = next;
				current_node = current_node->brother;
			}
            break;
		case VECTOR:
			stype->type = VECTOR;
			
			current_node = root->child;
			
			expr_ok = expr(current_node, f_loc_env, current_schema);
			current_node = current_node->brother;
			count++;
			while (current_node){
				Pschema next = new_schema_node(-1);
				expr_ok = expr_ok && expr(current_node, f_loc_env, next);
				if(!are_compatible(next,current_schema)){
					semantic_error(current_node, "Type Error, Vector type are non uniform");
					break;
				}
				current_node = current_node->brother;
				count++;
			}
			stype->size = count;
			stype->p1 = current_schema;
            break;
	}
	return expr_ok;
}
int func_call(Pnode root, Phash_node f_loc_env, Pschema stype){
    
}

int cond_expr(Pnode root, Phash_node f_loc_env, Pschema stype){

	Pnode main_expr_node = root->child;
	Pnode first_expr_node = main_expr_node->brother;
	Pnode elsif_expr_node = first_expr_node->brother;
	Pnode else_expr_node = elsif_expr_node->brother;

	//check contraint on conditional clause
	Pschema main_expr_type = new_schema_node(-1);;
	int main_expr_ok = expr(main_expr_node, f_loc_env, main_expr_type);

	if (main_expr_type->type!=BOOL){
		semantic_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}
    
	//check contraint on first and last alternative
	Pschema first_expr_type = stype;

	int first_expr_ok = expr(first_expr_node, f_loc_env, first_expr_type);

	Pschema else_expr_type = new_schema_node(-1);
	int else_expr_ok = expr(else_expr_node, f_loc_env, else_expr_type);

	if (!are_compatible(first_expr_type, else_expr_type)){
		semantic_error(else_expr_node, "Type error, alternatives are of different type\n");
	}
    
	//check contraint on elsif part
	Pschema elsif_expr_type = new_schema_node(-1);
	int elsif_expr_ok = elsif_expr_list_opt(elsif_expr_node, f_loc_env, elsif_expr_type);
	
	if (elsif_expr_type != NULL && !are_compatible(first_expr_type, elsif_expr_type)){
		semantic_error(elsif_expr_node, "Type error, alternatives are of different type\n");
	}
    
	return main_expr_ok && first_expr_ok && elsif_expr_ok && else_expr_ok;
}

int elsif_expr_list_opt(Pnode root, Phash_node f_loc_env, Pschema stype){
	if (root->child == NULL){
		stype = NULL;
		return 1;
	}
    
	Pnode main_expr_node = root->child;
	Pnode expr_node = main_expr_node->brother;
	Pnode elsif_expr_node = expr_node->brother;
    
	//check contraint on conditional clause
    Pschema main_expr_type = new_schema_node(-1);;
    int main_expr_ok = expr(main_expr_node, f_loc_env, main_expr_type);
    
    if (main_expr_type->type!=BOOL){
		semantic_error(main_expr_node, "Type Error, expected BOOL in conditional clause\n");
	}
    
	//check contraint on first and other elsif alternative recursively
    Pschema expr_type = stype;
    int expr_ok = expr(expr_node, f_loc_env, expr_type);
    
    Pschema elsif_expr_type = new_schema_node(-1);
    int elsif_expr_ok = elsif_expr_list_opt(elsif_expr_node, f_loc_env, elsif_expr_type);
	
	if (elsif_expr_type != NULL && !are_compatible(expr_type, elsif_expr_type)){
		semantic_error(elsif_expr_node, "type error, alternatives are of different type\n");
	}
    
	return main_expr_ok && expr_ok && elsif_expr_ok;
    
}
int built_in_call(Pnode root, Phash_node f_loc_env, Pschema stype){
	Pschema built_in_call_type = new_schema_node(-1);
	int built_in_call_ok = expr(root->child, f_loc_env, built_in_call_type);

	switch(root->qualifier){
		case TOINT:
			if(built_in_call_type->type != REAL){
				semantic_error(root->child, "Type error, expected REAL");
			}
			stype->type = INT;
		break;
		case TOREAL:
			if(built_in_call_type->type != INT){
				semantic_error(root->child, "Type error, expected INT");
			}
			stype->type = REAL;
		break;
	}
	return built_in_call_ok;
    
}

int expr(Pnode root, Phash_node f_loc_env, Pschema stype){
	int expr_ok;
    Class not_used;
    
	switch(root->type){
		case T_CHARCONST:
			stype->type = CHAR;
			break;
		case T_INTCONST:
			stype->type = INT;
			break;
		case T_REALCONST:
			stype->type = REAL;
			break;
		case T_STRCONST:
			stype->type = STRING;
			break;
		case T_BOOLCONST:
			stype->type = BOOL;
			break;
		case T_NONTERMINAL:
            switch(root->value.ival){
                case NLEFT_HAND_SIDE:
                    expr_ok = left_hand_side(root, f_loc_env, stype, &not_used);
                    break;
                case NMATH_EXPR:
                    expr_ok = math_expr(root, f_loc_env, stype);
                    break;
                case NLOGIC_EXPR:
                    expr_ok = logic_expr(root, f_loc_env, stype);
                    break;
                case NREL_EXPR:
                    expr_ok = rel_expr(root, f_loc_env, stype);
                    break;
                case NNEG_EXPR:
                    expr_ok = neg_expr(root, f_loc_env, stype);
                    break;
                case NWR_EXPR:
                    expr_ok = wr_expr(root, f_loc_env, stype);
                    break;
                case NRD_EXPR:
                    expr_ok = rd_expr(root, f_loc_env, stype);
                    break;
                case NINSTANCE_EXPR:
                    expr_ok = instance_expr(root, f_loc_env, stype);
                    break;
                case NFUNC_CALL:
                    expr_ok = func_call(root, f_loc_env, stype);
                    break;
                case NCOND_EXPR:
                    expr_ok = cond_expr(root, f_loc_env, stype);
                    break;
                case NBUILT_IN_CALL:
                    expr_ok = built_in_call(root, f_loc_env, stype);
                    break;
                default:
                    semantic_error(root, "Some weird nonterminal node in expr\n");
            }
            break;
		default:
			semantic_error(root, "Some weird terminal node in expr\n");
	}
	return expr_ok;
}

void semantic_error(Pnode node, char * msg){
    fprintf(stderr, "Row: %d Semantic error: %s", node->row, msg);
    exit(EXIT_FAILURE);
}