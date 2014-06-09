#include "semantic.h"

typedef enum{
	sem_char,
	sem_int,
	sem_real,
	sem_string,
	sem_bool,	
	sem_vector,
	sem_struct
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
int decl_list_opt(){

}
int decl(){

}
int id_list(){

}
int domain(){

}
int struct_domain(){

}
int vector_domain(){

}
int type_sect_opt(){

}
int var_sect_opt(){

}
int const_sect_opt(){

}
int func_list_opt(){

}
int func_body(){

}
int stat_list(){

}
int stat(){

}
int assign_stat(){

}
int left_hand_side(){

}
int fielding(){

}
int indexing(){

}
int if_stat(){

}
int elsif_stat_list_opt(){

}
int while_stat(){

}
int for_stat(){

}
int foreach_stat(){

}
int return_stat(){

}
int read_stat(){

}
int specifier_opt(){

}
int write_stat(){

}
int math_expr(){

}
int logic_expr(){

}
int rel_expr(){

}
int neg_expr(){

}
int wr_expr(){

}
int rd_expr(){

}
int instance_expr(){

}
int func_call(){

}
int cond_expr(){

}
int elsif_expr_list_opt(){

}
int built_in_call(){

}

