#ifndef SOL_compilers_code_gen_h
#define SOL_compilers_code_gen_h

#include "def.h"
#include "hash_table.h"
#include <stdio.h>


typedef enum{
    S_NEW,      //NEW object-size
    S_NEWS,     //NEWS object-size
    S_LDC,      //LDC 'c' LDC '1'
    S_LDI,      //LDI 25
    S_LDR,      //LDR 3.14
    S_LDS,      //LDS "alpha"
    S_LOD,      // LOD = env-offset, oid
    S_CAT,      //CAT = num-fields, struct-size  CAT = num-elements, vector-size
    S_LDA,      //LDA = env-offset, oid
    S_FDA,      //FDA (field address) = field-offset
    S_EIL,      //EIL, SIL (indirect load, either embedded or on stack) = field-size
    S_SIL,      //EIL, SIL (indirect load, either embedded or on stack) = field-size
    S_IXA,      //IXA (indexed address) = elem-size
    S_STO,      //STO = env-offset, oid
    S_IST,      //IST = operator without arguments
    S_JMF,      //
    S_JMP,
    S_EQU,
    S_NEQ,
    S_CGT,
    S_IGT,
    S_RGT,
    S_SGT,
    S_CGE,
    S_IGE,
    S_RGE,
    S_SGE,
    S_CLT,
    S_ILT,
    S_RLT,
    S_SLT,
    S_CLE,
    S_ILE,
    S_RLE,
    S_SLE,
    S_IN,
    S_IPLUS,
    S_RPLUS,
    S_IMINUS,
    S_RMINUS,
    S_ITIMES,
    S_RTIMES,
    S_IDIV,
    S_RDIV,
    S_IUMI,
    S_RUMI,
    S_NEG,
    S_WR,
    S_FWR,
    S_PUSH,
    S_GOTO,
    S_POP,
    S_RD,
    S_FRD,
    S_TOINT,
    S_TOREAL,
    S_READ,
    S_FREAD,    //READ offset-env oid format
    S_WRITE,
    S_FWRITE,
    S_FUNC,
    S_HALT,
    S_SCODE,
    S_RETURN,
    S_FAKE_RETURN,
    S_NOOP,
    S_FAKE_GOTO
} Operator;


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
