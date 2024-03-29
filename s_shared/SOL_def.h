#ifndef SOL_def_h
#define SOL_def_h

#include <stdio.h>
#include <stdlib.h>

typedef union{
	char cval;
	int ival;
	float rval;
	char * sval;
} Value;

typedef enum{
    S_NEW,
    S_NEWS,
    S_LDC,
    S_LDI,
    S_LDR,
    S_LDS,
    S_LOD,
    S_CAT,
    S_LDA,
    S_FDA,
    S_EIL,
    S_SIL,
    S_IXA,
    S_STO,
    S_IST,
    S_JMF,
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
    S_FREAD,
    S_WRITE,
    S_FWRITE,
    S_FUNC,
    S_HALT,
    S_SCODE,
    S_RETURN,
    S_FAKE_RETURN,
    S_NOOP,
    S_FAKE_GOTO,
    S_FAKE_PUSH
} Operator;

#endif
