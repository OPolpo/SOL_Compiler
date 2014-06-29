%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "s_exec.h"
#include "s_machine.h"
#include "../s_compiler/schema.h"
#include "../s_compiler/parser.h"

#define YYSTYPE Pschema

extern Value lexval;
extern Str_c_node ** format_stringtable;
Pschema root = NULL;

%}
%token FORMAT_LEX_ID FORMAT_LEX_INT FORMAT_LEX_ERROR
%%

format_string : format {root = $$; $$ = $1;}

format : atomic_format
        | struct_format
        | vector_format

atomic_format   : 'c' {$$ = new_schema_node(CHAR);}
                | 'i' {$$ = new_schema_node(INT);}
                | 'r' {$$ = new_schema_node(REAL);}
                | 's' {$$ = new_schema_node(STRING);}
                | 'b' {$$ = new_schema_node(BOOL);}

struct_format : '(' attr attr_list ')' {$$ = new_schema_node(STRUCT);
                                        $$->p1 = $2;
                                        $2->p2 = $3;}

attr_list : ',' attr attr_list {$$ = $2;
                                $2->p2 = $3;}
        | /*eps*/ {$$ = NULL;}

attr : FORMAT_LEX_ID ':' format {$$ = $3; $3->id = $1;}

vector_format : '[' FORMAT_LEX_INT ',' format ']' {$$ = new_schema_node(VECTOR);
                                                    $$->size = $2;
                                                    $$->p1 = $4;}

%%

int yyerror(){
    fprintf(stderr, "Error while parsing the format.\n");
    return -1;
}

int parse_format(char *format){
    if((result = yyparse()) == 0)
        print_sch();
    
    return 0;
}