%{
#include <stdlib.h>
#include "formatted_parser.h"
#include "s_machine.h"

Value lexval;
%}
%option	noyywrap


spacing		([ \t])+
eol			\n
letter		[a-zA-Z]
digit		[0-9]
id			{letter}({letter}|{digit})*:
num 		{digit}+
sugar 		[\(\)\[\]\{\}".":,;]
charconst 	\'.\'
intconst 	{num}
realconst	{num}?"."{num}
boolconst	(true|false)
stringconst \"([^\"])*\"

%%

{boolconst}		{lexval.cval = (yytext[0] == 'f' ? '0' : '1'); return(BOOLCONST);}
{intconst}		{lexval.ival = atoi(yytext); return(INTCONST);}
{realconst}		{lexval.rval = atof(yytext); return(REALCONST);}
{charconst}		{lexval.ival = yytext[1]; return(CHARCONST);}
{stringconst}	{lexval.sval = insert_str_c(yytext); return(STRCONST);}
{sugar}			{return(yytext[0]);}
{id}			{yytext[strlen(yytext)-1]=0;lexval.sval = insert_str_c(yytext); return FORMATTED_LEX_ID;}
.				{return (FORMATTED_LEX_ERROR);}
