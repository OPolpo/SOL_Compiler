#ifndef _def_h
#define _def_h

#include <stdio.h>
#include <stdlib.h>

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
	NELSIF_STAT_LIS_OPT,
	NWHILE_STAT,
	NFOR_STAT,
	NFOREACH_STAT,
	NRETURN_STAT,
	NREAD_STAT,
	NSPECIFIER_OPT,
	NWRITE_STAT,
	NCOND_EXP,
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

typedef union{
	int ival;
	float rval;
	char *sval;
	enum {FALSE, TRUE} bval;
} Value;

typedef struct snode{
	Typenode type;
	Value value;
	struct snode *child, *brother;
} Node;

typedef Node *Pnode;

char *newstring(char*);

Pnode nontermode(Nonterminal),
	  idnode(),
	  iconstnode(),
	  sconstnode(),
	  rconstnode(),
	  bconstnode(),
      atomicdomainnode(int domain),
	  newnode(Typenode);

int yylex (void);
int yyerror ();

#endif