#ifndef SOL_compilers_code_gen_h
#define SOL_compilers_code_gen_h

#include "def.h"


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
    S_SCODE
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
Code makecode3(Operator op, int arg1, int arg2, int arg3);
Code make_push_pop(int size, int chain, int entry);
Code make_ldc(char c);
Code make_ldi(int i);
Code make_ldr(float r);
Code make_lds(char *s);
void print_code(Code * code);
void print_stat(FILE * stream, Stat * stat);

#endif
