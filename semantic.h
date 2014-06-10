#ifndef _semantic_h
#define _semantic_h

#include "def.h"
#include "parser.h"
#include "hash_table.h"
#include "symbol_table.h"

// typedef enum{
// 	SEM_CHAR,
// 	SEM_INT,
// 	SEM_REAL,
// 	SEM_STRING,
// 	SEM_BOOL,
// 	SEM_VECTOR,
// 	SEM_STRUCT
// } Sem_type;


// char* tabsem_types[] ={
// 	"SEM_CHAR",
// 	"SEM_INT",
// 	"SEM_REAL",
// 	"SEM_STRING",
// 	"SEM_BOOL",	
// 	"SEM_VECTOR",
// 	"SEM_STRUCT"
// };


int program(Pnode root, Phash_node f_loc_env, int not_first);
int func_decl(Pnode root, Phash_node f_loc_env, int not_first);
int decl_list_opt(Pnode root, Phash_node f_loc_env);
int decl(Pnode root, Phash_node f_loc_env);
int id_list(Pnode root, Phash_node f_loc_env);
int domain(Pnode root, Phash_node f_loc_env, Pschema stype);
int struct_domain(Pnode root, Phash_node f_loc_env);
int vector_domain(Pnode root, Phash_node f_loc_env);
int type_sect_opt(Pnode root, Phash_node f_loc_env);
int var_sect_opt(Pnode root, Phash_node f_loc_env);
int const_sect_opt(Pnode root, Phash_node f_loc_env);
int func_list_opt(Pnode root, Phash_node f_loc_env);
int func_body(Pnode root, Phash_node f_loc_env);
int stat_list(Pnode root, Phash_node f_loc_env);
int stat(Pnode root, Phash_node f_loc_env);
int assign_stat(Pnode root, Phash_node f_loc_env);
int left_hand_side(Pnode root, Phash_node f_loc_env, Pschema type, Class * lhs_class);
int fielding(Pnode root, Phash_node f_loc_env, Pschema type, Class * lhs_class);
int indexing(Pnode root, Phash_node f_loc_env, Pschema type, Class * lhs_class);
int if_stat(Pnode root, Phash_node f_loc_env);
int elsif_stat_list_opt(Pnode root, Phash_node f_loc_env);
int while_stat(Pnode root, Phash_node f_loc_env);
int for_stat(Pnode root, Phash_node f_loc_env);
int foreach_stat(Pnode root, Phash_node f_loc_env);
int return_stat(Pnode root, Phash_node f_loc_env);
int read_stat(Pnode root, Phash_node f_loc_env);
int specifier_opt(Pnode specifier_opt, Phash_node f_loc_env);
int write_stat(Pnode root, Phash_node f_loc_env);
int math_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int logic_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int rel_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int neg_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int wr_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int rd_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int instance_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int func_call(Pnode root, Phash_node f_loc_env, Pschema stype);
int cond_expr(Pnode root, Phash_node f_loc_env, Pschema stype);
int elsif_expr_list_opt(Pnode root, Phash_node f_loc_env, Pschema stype);
int built_in_call(Pnode root, Phash_node f_loc_env, Pschema stype);

int expr(Pnode root, Phash_node f_loc_env, Pschema stype);
void semantic_error(char * msg );
#endif