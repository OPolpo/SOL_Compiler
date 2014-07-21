#ifndef _semantic_h
#define _semantic_h

#include "def.h"
#include "hash_table.h"
#include "symbol_table.h"
#include "code_gen.h"
#include "../s_shared/schema.h"

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


int sem_program(Pnode root, Phash_node f_loc_env, Code * code);
int sem_func_decl(Pnode root, Phash_node f_loc_env, int not_first, Code * code, Poid2address * func_table);
int sem_decl_list_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects);
int sem_decl(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int * num_objects);
int sem_id_list(Pnode root, int * num_objects);
int sem_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_struct_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_vector_domain(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_var_sect_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects);
int sem_const_sect_opt(Pnode root, Phash_node f_loc_env, Code * code, int * num_objects);
int sem_func_list_opt(Pnode root, Phash_node f_loc_env, Code * code, Poid2address * func_table);

int sem_func_body(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux);

int sem_stat_list(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux);
int sem_stat(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux);
int sem_assign_stat(Pnode root, Phash_node f_loc_env, Code * code);
int sem_left_hand_side(Pnode root, Phash_node f_loc_env, Pschema * type, Class * lhs_class, Code * code, int is_addr, int * is_s);
int sem_fielding(Pnode root, Phash_node f_loc_env, Pschema * type, Class * lhs_class, Code * code, int is_addr, int * is_s);
int sem_indexing(Pnode root, Phash_node f_loc_env, Pschema * type, Class * lhs_class, Code * code, int is_addr, int * is_s);
int sem_if_stat(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux);
int sem_elsif_stat_list_opt(Pnode root, Phash_node f_loc_env, int * w_return, Code * code, Code * code_new_aux, int * offset_to_exit);
int sem_while_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux);

int sem_for_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux);
int sem_foreach_stat(Pnode root, Phash_node f_loc_env, Code * code, Code * code_new_aux);

int sem_return_stat(Pnode root, Phash_node f_loc_env, Code * code);
int sem_read_stat(Pnode root, Phash_node f_loc_env, Code * code);
int sem_specifier_opt(Pnode specifier_opt, Phash_node f_loc_env, Code * code, int * is_null);
int sem_write_stat(Pnode root, Phash_node f_loc_env, Code * code);
int sem_math_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_logic_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_rel_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_neg_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_wr_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_rd_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_instance_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_func_call(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_cond_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
int sem_elsif_expr_list_opt(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int * offset_to_exit);
int sem_built_in_call(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code);
void cleanup_return(Stat * start, int code_len, Code * code);
void cleanup_fake(Code * code, Poid2address * func_table);

int sem_expr(Pnode root, Phash_node f_loc_env, Pschema * stype, Code * code, int is_addr);
void sem_error(Pnode node, char * msg);

#endif