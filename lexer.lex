%{
#include <stdlib.h>
#include "def.h"
Value lexval;
double row_counter = 1;
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
{eol}			row_counter++;

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
"<"				{lexval.intval = LT; return(REL_OP);}
"<="			{lexval.intval = LE; return(REL_OP);}
"=="			{lexval.intval = EQ; return(REL_OP);}
"<>"			{lexval.intval = NE; return(REL_OP);}
">"				{lexval.intval = GT; return(REL_OP);}
">="			{lexval.intval = GE; return(REL_OP);}
in				{lexval.intval = IN; return(REL_OP);}
"+"				{return(yytext[0]);}
"-"				{return(yytext[0]);}
"*"				{return(yytext[0]);}
"/"				{return(yytext[0]);}
"="				{return (ASSIGN);}
and				{lexval.intval = AND; return(LOG_OP);}
or				{lexval.intval = OR; return(LOG_OP);}
not				{lexval.intval = NOT; return(LOG_OP);}
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
					lexval.boolval = (yytext[0] == 'f' ? FALSE : TRUE);
					return(BOOL_CONST);
				}
{intconst}		{lexval.intval = atoi(yytext); return(INT_CONST);}
{realconst}		{lexval.realval = atof(yytext); return(REAL_CONST);}
{charconst}		{lexval.intval = yytext[1]; return(CHAR_CONST);}
{stringconst}	{lexval.stringval = store_str(yytext); return(STRING_CONST);}
{id}			{lexval.stringval = store_id(yytext); return(ID);}

.				{return (ERROR);}

%%

char *newstring(char *s) {
	char *p;
 	p = malloc(strlen(s)+1);
 	strcpy(p, s);
 	return(p);
}
int main()
{	
 	printf("%.0f:\t%d\n", row_counter, yylex());
 	return 0;
}
