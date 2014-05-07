%{
#include <stdlib.h>
#include "def.h"
value lexval;
%}
%option	noyywrap


spacing		([ \t\n])+
letter		[a-­zA-Z]
digit		[0­]
id			{letter}({letter}|{digit})*
num 		{digit}+
sugar 		[(){}:,;]
charconst 	\'.\'
intconst 	{num}
realconst 	{num}?.{num}
stringconst \"([^\"])*\"
boolconst 	(true|false)


%%


{spacing}		;
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
"<"				{lexval = LT; return(REL_OP);}
"<="			{lexval = LE; return(REL_OP);}
"=="			{lexval = EQ; return(REL_OP);}
"<>"			{lexval = NE; return(REL_OP);}
">"				{lexval = GT; return(REL_OP);}
">="			{lexval = GE; return(REL_OP);}
in				{lexval = IN; return(REL_OP);}
"+"				{return(yytext[0]);}
"-"				{return(yytext[0]);}
"*"				{return(yytext[0]);}
"/"				{return(yytext[0]);}
and				{lexval = AND; return(LOG_OP);}
or				{lexval = OR; return(LOG_OP);}
not				{lexval = NOT; return(LOG_OP);}
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
endforeach		{return (ENDFOREA);}
return			{return (RETURN);}
read			{return (READ);}
rd				{return (RD);}
write			{return (WRITE);}
wr				{return (WR);}


{id}			{lexval.stringval = store_id(yytext); return(ID);}
{intconst}		{lexval.intval = atoi(yytext); return(INT_CONST);}
{realconst}		{lexval = atof(yytext); return(REAL_CONST);}
{charconst}		{lexval = yytext[1]; return(CHAR_CONST);}
{stringconst}	{lexval.stringval = store_str(yytext); return(STRCONST);}
{boolconst}		{
					lexval.boolval = (yytext[0] == 'f' ? FALSE : TRUE);
					return(BOOLCONST);
				}


%%

char *newstring(char *s) {
	char *p;
 	p = malloc(strlen(s)+1);
 	strcpy(p, s);
 	return(p);
}

int store_id(){
	int line;
	//if((line = lookup(yytext)) == 0) line = insert(yytext);
  	return(line);
}