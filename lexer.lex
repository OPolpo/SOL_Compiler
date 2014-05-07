%{
#include <stdlib.h>
#include "def.h" /* IF, THEN, ELSE, ID, NUM, RELOP, LT, LE, EQ, NE, GT, GE */ int lexval;
%}

spacing {[ \t\n]}+
letter [A­-Za-­z]
digit [0­-9]
id {letter}({letter}|{digit})*
num {digit}+
sugar [(){}:,;]
charconst \'.\'
intconst {num}
realconst {num}?.{num}
stringconst \"([^\"])*\"
boolconst (true|false)


%%
{spacing} ;
if {return(IF);}
then {return(THEN);}
else {return(ELSE);}
{id} {lexval = store_id(); return(ID);}
{num} {lexval = atoi(yytext); return(NUM);}
"<" {lexval = LT; return(RELOP);}
"<=" {lexval = LE; return(RELOP);}
"=" {lexval = EQ; return(RELOP);}
"<>" {lexval = NE; return(RELOP);}
">" {lexval = GT; return(RELOP);}
">=" {lexval = GE; return(RELOP);}
%%
int store_id() /* symbol table without keywords */
{ int line;
if((line = lookup(yytext)) == 0) line = insert(yytext);
  return(line);
}