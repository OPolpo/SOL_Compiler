#ifndef _semantic_h
#define _semantic_h

#include "def.h"

int program(Pnode root);
int func_decl(Pnode root);
int decl_list_opt();
int decl();
int id_list();
int domain();
int struct_domain();
int vector_domain();
int type_sect_opt();
int var_sect_opt();
int const_sect_opt();
int func_list_opt();
int func_body();
int stat_list();
int stat();
int assign_stat();
int left_hand_side();
int fielding();
int indexing();
int if_stat();
int elsif_stat_list_opt();
int while_stat();
int for_stat();
int foreach_stat();
int return_stat();
int read_stat();
int specifier_opt();
int write_stat();
int math_expr();
int logic_expr();
int rel_expr();
int neg_expr();
int wr_expr();
int rd_expr();
int instance_expr();
int func_call();
int cond_expr();
int elsif_expr_list_opt();
int built_in_call();
#endif