#ifndef SOL_compilers_code_gen_h
#define SOL_compilers_code_gen_h

#include "hash_table.h"
#include "../s_shared/SOL_def.h"
#include <stdio.h>

typedef struct sstat{
    int address;
    Operator op;
    Value args[3];
    struct sstat * next;
} Stat;

typedef struct scode{
    Stat * head;
    int size;
    Stat * tail;
}Code;

void relocate_address(Code code, int offset);
Code appcode(Code code1, Code code2);
Code endcode();
Code concode(Code code1, Code code2, ...);
Stat *newstat(Operator op);
Code makecode(Operator op);
Code makecode1(Operator op, int arg);
Code makecode2(Operator op, int arg1, int arg2);
Code makecode_str(Operator op, char * s);
Code makecode_xread(Operator op, int arg1, int arg2, char * arg3);
Code make_push_pop(int param, int size, int chain, int entry);
Code make_ldc(char c);
Code make_ldi(int i);
Code make_ldr(float r);
Code make_lds(char *s);
Code makecode_push(int arg1, int arg2, int arg3);
void print_code(FILE * stream, Code * code);
void print_stat(FILE * stream, Stat * stat);

//Code Stack
typedef struct snode_stack{
    Code * code;
    struct snode_stack *next;
} Stack_node_code;

void StackPush(Stack_node_code ** stackPtr, Code * element);
Code * StackPop(Stack_node_code ** stackPtr);

typedef struct sfunc_oid{
    int oid;
    int * address;
    struct sfunc_oid * next;
} Oid2address, *Poid2address;

Poid2address * new_o2a_table();
int insert_o2a(Poid2address p, Poid2address * table);
int get_f_addr_by_oid(int oid, Poid2address * table);
Poid2address new_o2a(int oid, int * addr);
void destroy_o2a(Poid2address * table);
//void free_list(Poid2address list);
void destroy_code(Code *code);


#endif
