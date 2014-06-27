%{
#include <string.h>
#include "instructions.h"
#include "format_parser.h"
#include "s_machine.h"

Arg lexval;
Str_c_node **  format_stringtable;
%}
%option noyywrap
spacing     ([ \t])+
digit       [0-9]
letter      [A-Za-z]
string      {letter}+
sugar       [\[\]\(\)\:\,]
%%
{spacing}   ;
{digit}+    {lexval.ival = atoi(yytext); return(FORMAT_LEX_INT);}
{sugar}     {return yytext[0];}
{string}    {
                if(strlen(yytext)==1){
                    lexval.cval = yytext[0];
                    return(FORMAT_LEX_CHAR); 
                } else{ 
                    lexval.sval = insert_str_c(yytext, format_stringtable); 
                    return(FORMAT_LEX_STRING);
                }
            }
.           {return(FORMAT_LEX_ERROR);}
%%