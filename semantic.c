#include "semantic.h"

typedef enum{
	SEM_CHAR,
	SEM_INT,
	SEM_REAL,
	SEM_STRING,
	SEM_BOOL,	
	SEM_VECTOR,
	SEM_STRUCT
} Sem_type;


int program(Pnode root){
	return func_decl(root->child);
}
int func_decl(Pnode root){
	Pnode id = root->child;
	Pnode current = id->brother;
	int decl_list_opt_ok = decl_list_opt(current);
	current = current->brother;
	int domain_ok = domain(current);
	current = current->brother;
	int type_sect_opt_ok = type_sect_opt(current);
	current = current->brother;
	int var_sect_opt_ok = var_sect_opt(current);
	current = current->brother;
	int const_sect_opt_ok = const_sect_opt(current);
	current = current->brother;
	int func_list_opt_ok = func_list_opt(current);
	current = current->brother;
	int func_body_ok = func_body(current);

}
int decl_list_opt(Pnode root){
	Pnode current = root->child;
	int decl_list_opt_ok = decl(current); //checked 2 time but it's just and and
	while(current != NULL){
		decl_list_opt_ok = decl_list_opt_ok && decl(current);
		current = current->brother;
	}
	return current;
}
int decl(Pnode root){

}
int id_list(Pnode root){

}
int domain(Pnode root){

}
int struct_domain(Pnode root){

}
int vector_domain(Pnode root){

}
int type_sect_opt(Pnode root){

}
int var_sect_opt(Pnode root){

}
int const_sect_opt(Pnode root){

}
int func_list_opt(Pnode root){

}
int func_body(Pnode root){

}
int stat_list(Pnode root){

}
int stat(Pnode root){

}
int assign_stat(Pnode root){

}
int left_hand_side(Pnode root, Sem_type type){

}
int fielding(Pnode root){

}
int indexing(Pnode root){

}
int if_stat(Pnode root){

}
int elsif_stat_list_opt(Pnode root){

}
int while_stat(Pnode root){

}
int for_stat(Pnode root){

}
int foreach_stat(Pnode root){

}
int return_stat(Pnode root){

}
int read_stat(Pnode root){

}
int specifier_opt(Pnode root){

}
int write_stat(Pnode root){

}
int math_expr(Pnode root){

}
int logic_expr(Pnode root, Sem_type * stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Sem_type expr1_type, expr2_type;
	expr1_ok = expr(expr1, &expr1_type);
	if(expr1_type != SEM_BOOL)
		semantic_error("Type mismatch, expected BOOL\n");
	expr2_ok = expr(expr2, &expr2_type);
	if(expr2_type != SEM_BOOL)
		semantic_error("Type mismatch, expected BOOL\n");
	*stype = SEM_BOOL;
	return expr1_ok && expr2_ok;
}
int rel_expr(Pnode root, Sem_type * stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Sem_type expr1_type, expr2_type;
	expr1_ok = expr(expr1, &expr1_type);
	expr2_ok = expr(expr2, &expr2_type);
	switch(root->qualifier){
		case EQ:
		case NE:

			break;
		case '>':
		case GE:
		case '<':
		case LE:
			break;
		case IN:
			break;
		default:
			semantic_error("Some weird qualification in relational expression\n");
	}


	if(expr2_type != SEM_BOOL)
		semantic_error("Type mismatch, expected BOOL\n");
	*stype = SEM_BOOL;
	return expr1_ok && expr2_ok;
}
int neg_expr(Pnode root){

}
int wr_expr(Pnode root){

}
int rd_expr(Pnode root){

}
int instance_expr(Pnode root){

}
int func_call(Pnode root){

}
int cond_expr(Pnode root){

}
int elsif_expr_list_opt(Pnode root){

}
int built_in_call(Pnode root){

}

int expr(Pnode root, Sem_type * stype){
	int expr_ok;
	switch(root->type){
		case T_CHARCONST:
			stype = SEM_CHAR;
			break;
		case T_INTCONST:
			stype = SEM_INT;
			break;
		case T_REALCONST:
			stype = SEM_REAL;
			break;
		case T_STRCONST:
			stype = SEM_STRING;
			break;
		case T_BOOLCONST:
			stype = SEM_BOOL;
			break;
		case T_NONTERMINAL:
		switch(root->value.ival){
			case NLEFT_HAND_SIDE:
				expr_ok = left_hand_side(root, stype);
				break;
			case NMATH_EXPR:
				expr_ok = math_expr(root, stype);
				break;
			case NLOGIC_EXPR:
				expr_ok = logic_expr(root, stype);
				break;
			case NREL_EXPR:
				expr_ok = rel_expr(root, stype);
				break;
			case NNEG_EXPR:
				expr_ok = neg_expr(root, stype);
				break;
			case NWR_EXPR:
				expr_ok = wr_expr(root, stype);
				break;
			case NRD_EXPR:
				expr_ok = rd_expr(root, stype);
				break;
			case NINSTANCE_EXPR:
				expr_ok = instance_expr(root, stype);
				break;
			case NFUNC_CALL:
				expr_ok = func_call(root, stype);
				break;
			case NCOND_EXPR:
				expr_ok = cond_expr(root, stype);
				break;
			case NBUILT_IN_CALL:
				expr_ok = built_in_call(root, stype);
				break;
			default:
				semantic_error("Some weird nonterminal node in expr\n");
		}
		break;
		default:
			semantic_error("Some weird terminal node in expr\n");
	}
	return expr_ok;
}


