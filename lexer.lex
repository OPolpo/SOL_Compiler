%{
#include <stdlib.h>
#include <stdio.h>
#include "def.h"
int lexval;
%}
delimiter [ \t\n]
spacing {delimiter}+
letter [A­Za­z]
digit [0­9]
id {letter}({letter}|{digit})*
num {digit}+


%%


{spacing}    ;
if		{return(IF);}
then	{return(THEN);}
elsif	{return(ELSIF);}
endif	{return(ENDIF);}
else	{return(ELSE);}


{id} {lexval = store_id(); return(ID);}
{num} {lexval = atoi(yytext); return(NUM);}
"<" {lexval = LT; return(REL_OP);}
"<=" {lexval = LE; return(REL_OP);}
"=" {lexval = EQ; return(REL_OP);}
"<>" {lexval = NE; return(REL_OP);}
">" {lexval = GT; return(REL_OP);}
">=" {lexval = GE; return(REL_OP);}


%%


int store_id() /* symbol table without keywords */
{ int line;
if((line = lookup(yytext)) == 0) line = insert(yytext);
  return(line);
}