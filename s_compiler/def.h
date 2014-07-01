#ifndef _def_h
#define _def_h

#include <stdio.h>
#include <stdlib.h>
#include "../s_shared/SOL_def.h"

typedef enum{	
	NPROGRAM,
	NFUNC_DECL,
	NDECL_LIST_OPT,
	NDECL,
	NID_LIST,
	NDOMAIN,
	NSTRUCT_DOMAIN,
	NVECTOR_DOMAIN,
	NTYPE_SECT_OPT,
	NVAR_SECT_OPT,
	NCONST_SECT_OPT,
	NFUNC_LIST_OPT,
	NFUNC_BODY,
	NSTAT_LIST,
	NSTAT,
	NASSIGN_STAT,
	NLEFT_HAND_SIDE,
	NFIELDING,
	NINDEXING,
	NIF_STAT,
	NELSIF_STAT_LIST_OPT,
	NWHILE_STAT,
	NFOR_STAT,
	NFOREACH_STAT,
	NRETURN_STAT,
	NREAD_STAT,
	NSPECIFIER_OPT,
	NWRITE_STAT,
    NMATH_EXPR,
    NLOGIC_EXPR,
    NREL_EXPR,
    NNEG_EXPR,
    NWR_EXPR,
    NRD_EXPR,
    NINSTANCE_EXPR,
    NFUNC_CALL,
	NCOND_EXPR,
	NELSIF_EXPR_LIST_OPT,
	NBUILT_IN_CALL
    
} Nonterminal;

typedef enum{
	T_ID,
	T_ATOMIC_DOMAIN,
	T_CHARCONST,
	T_INTCONST,
	T_REALCONST,
	T_STRCONST,
	T_BOOLCONST,
	T_NONTERMINAL
} Typenode;

typedef struct snode{
	Typenode type;
	Value value;
    int qualifier;
    int row;
	struct snode *child, *brother;
} Node, * Pnode;

char *newstring(char*);

Pnode nontermode(Nonterminal),
	  idnode(),
	  iconstnode(),
	  sconstnode(),
	  rconstnode(),
	  bconstnode(),
	  cconstnode(),
      atomicdomainnode(int domain),
	  newnode(Typenode);

int yylex (void);
int yyerror ();

#endif