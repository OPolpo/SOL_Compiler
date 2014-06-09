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


int program(Pnode root);
int func_decl(Pnode root);
int decl_list_opt();
int decl();
int id_list();
int domain(Pnode root, Pschema stype);
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
int math_expr(Pnode root, Pschema stype);
int logic_expr(Pnode root, Pschema stype);
int rel_expr(Pnode root, Pschema stype);
int neg_expr(Pnode root, Pschema stype);
int wr_expr(Pnode root, Pschema stype);
int rd_expr(Pnode root, Pschema stype);
int instance_expr(Pnode root, Pschema stype);
int func_call();
int cond_expr(Pnode root, Pschema stype);
int elsif_expr_list_opt();
int built_in_call(Pnode root, Pschema stype);

void semantic_error(char * msg);
int expr(Pnode root, Pschema stype);
#endif