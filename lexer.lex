%{
#include <stdlib.h>
#include "def_y.h"
#include "parser.h"

Value lexval;
unsigned int line = 1;
%}
%option	noyywrap


spacing		([ \t])+
eol			\n
letter		[a-­zA-Z]
digit		[0-9]
id			{letter}({letter}|{digit})*
num 		{digit}+
sugar 		[(){}:,;]
charconst 	\'.\'
intconst 	{num}
realconst	{num}?.{num}
boolconst	(true|false)
stringconst \"([^\"])*\"


%%


{spacing}		;
{eol}			line++;
{sugar}			{return(yytext[0]);}
char			{return (CHAR);}
int				{return (INT);}
real			{return (REAL);}
string			{return (STRING);}
bool			{return (BOOL);}
struct			{return (STRUCT);}
vector			{return (VECTOR);}
of				{return (OF);}
type			{return (TYPE);}
var				{return (VAR);}
const			{return (CONST);}
func			{return (FUNC);}
toint			{return (TOINT);}
toreal			{return (TOREAL);}
begin			{return (F_BEGIN);}
end				{return (F_END);}
"<"				{lexval.ival = '<'; return(REL_OP);}
"<="			{lexval.ival = LE; return(REL_OP);}
"=="			{lexval.ival = EQ; return(REL_OP);}
"<>"			{lexval.ival = NE; return(REL_OP);}
">"				{lexval.ival = '>'; return(REL_OP);}
">="			{lexval.ival = GE; return(REL_OP);}
in				{lexval.ival = IN; return(REL_OP);}
"+"				{return(yytext[0]);}
"-"				{return(yytext[0]);}
"*"				{return(yytext[0]);}
"/"				{return(yytext[0]);}
"="				{return(yytext[0]);}
and				{lexval.ival = AND; return(LOG_OP);}
or				{lexval.ival = OR; return(LOG_OP);}
not				{lexval.ival = NOT; return(LOG_OP);}
if				{return (IF);}
then			{return (THEN);}
else			{return (ELSE);}
elsif			{return (ELSIF);}
endif			{return (ENDIF);}
while			{return (WHILE);}
do				{return (DO);}
endwhile		{return (ENDWHILE);}
for				{return (FOR);}
to				{return (TO);}
endfor			{return (ENDFOR);}
foreach			{return (FOREACH);}
endforeach		{return (ENDFOREACH);}
return			{return (RETURN);}
read			{return (READ);}
rd				{return (RD);}
write			{return (WRITE);}
wr				{return (WR);}



{boolconst}		{
					lexval.bval = (yytext[0] == 'f' ? FALSE : TRUE);
					return(BOOLCONST);
				}
{intconst}		{lexval.ival = atoi(yytext); return(INTCONST);}
{realconst}		{lexval.rval = atof(yytext); return(REALCONST);}
{charconst}		{lexval.ival = yytext[1]; return(CHARCONST);}
{stringconst}	{lexval.sval = newstring(yytext); return(STRCONST);}
{id}			{lexval.sval = newstring(yytext); return(ID);}

.				{return (ERROR);}

%%


char *newstring(char *s) {
	char *p;
 	p = malloc(strlen(s)+1);
 	strcpy(p, s);
 	return(p);
}

/**
int main(){
	yylex();
	printf("%d:  %d\n",line,yylex());
}
**/