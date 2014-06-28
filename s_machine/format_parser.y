%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "s_exec.h"
#include "s_machine.h"

#define YYSTYPE Pformat
Pformat last_parsed_format = NULL;
    
extern Value lexval;
extern Str_c_node ** format_stringtable;

Pformat create_format(Type t){
    Pformat p;
    p = newmem(sizeof(Format));
    p->type = t;
    p->brother = NULL;
    p->child = NULL;
    p->id = NULL;
    p->size = 0;
    return p;
}

Pformat sizenode(){
    Pformat p = create_format(TY_ARRAY);
    p->size = lexval.ival;
    return p;
}

%}
%token FORMAT_LEX_STRING FORMAT_LEX_INT FORMAT_LEX_CHAR FORMAT_LEX_ERROR
%%

format_string : '"' format '"' {$$ = $2;}

format : simple_format {last_parsed_format = $$;} | complex_format {last_parsed_format = $$;};

simple_format : format_LEXER_CHAR {
    switch(lexval.cval){
        case 'i': $$ = create_format(T_INT); break;
        case 'r': $$ = create_format(T_REAL); break;
        case 'b': $$ = create_format(T_BOOL); break;
        case 'c': $$ = create_format(T_CHAR); break;
        case 's': $$ = create_format(T_STRING); break;
        default: yyerror(); break;
    }
};

complex_format : array_format | record_format;

array_format : '[' format_LEXER_INT {$$ = sizenode();} ',' format ']' {$$ = $3; $$->child = $5;};

record_format : '(' attr_format_list ')' {$$ = create_format(TY_RECORD); $$->child = $2;};

attr_format_list : attr_format 
                | attr_format ',' attr_format_list {$$ = $1; $$->brother = $3;}
;

attr_format : FORMAT_LEX_STRING {$$ = create_format(TY_ATTR); $$->id = lexval.sval;} ':' format {$$ = $2; $$->child = $4;}
                | format_LEXER_CHAR {
                    $$ = create_format(TY_ATTR); 
                    $$->id = newmem(2); 
                    $$->id[0] = lexval.cval;
                    $$->id[1] = '\0';
                    char *name = insert_str_c($$->id, format_stringtable); //todo
                    freemem($$->id, 2);
                    $$->id = name;
                } ':' format {$$ = $2; $$->child = $4;}
;

%%

int yyerror(){
    fprintf(stderr, "Error while parsing the format.\n");
    return -1;
}

int parse_format(char *format){
    format_scan_string(format, strlen(format));
    formatparse();
    /*
    if(root!=NULL) {
        print_format(root,0);
    } else {
        fprintf(stderr, "The parse tree is empty. \n");
    }
    */
    return 0;
}