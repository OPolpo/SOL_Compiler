%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "s_exec.h"
    #include "s_machine.h"
    #include "../s_compiler/schema.h"
    
    #define YYSTYPE Pschema
    
    typedef struct sformatted{
        Typeformatted type;
        Value value;
        char * id;
        struct sformatted *child, *brother;
    } Formatted, * Pformatted;
    
    typedef enum{
        F_VECTOR,
        F_STRUCT,
        F_CHARCONST,
        F_INTCONST,
        F_REALCONST,
        F_STRUCT,
        F_BOOLCONST
    }Typeformatted;
    
    extern Value lexval;
    extern Str_c_node ** format_stringtable;
    Pformatted root = NULL;
    
    Pformatted vectornode(){
        Pformatted p = newnode(F_VECTOR);
        return(p);
    }
    
    Pformatted structnode(){
        Pformatted p = newnode(F_STRUCT);
        return(p);
    }
    Pformatted iconstnode(){
        Pformatted p = newnode(F_INTCONST);
        p->value.ival = lexval.ival;
        return(p);
    }
    
    Pformatted cconstnode(){
        Pformatted p = newnode(F_CHARCONST);
        p->value.ival = lexval.cval;
        return(p);
    }
    
    Pformatted rconstnode(){
        Pformatted p = newnode(F_REALCONST);
        p->value.rval = lexval.rval;
        return(p);
    }
    
    Pformatted sconstnode(){
        Pformatted p = newnode(F_STRCONST);
        p->value.sval = lexval.sval;
        return(p);
    }
    
    Pformatted bconstnode(){
        Pformatted p = newnode(F_BOOLCONST);
        p->value.cval = lexval.cval;
        return(p);
    }
    
    Pformatted newnode(Typenode tnode){
        Pformatted p = malloc(sizeof(Node));
        p->type = tnode;
        p->child = p->brother = NULL;
        p->row = line;
        return(p);
    }
    
    
    %}
%token FORMATTED_LEX_ID F_INTCONST F_REALCONST F_STRCONST F_BOOLCONST FORMATTED_LEX_ERROR
%%

formatted_string : formatted {root = $$; $$ = $1;}

formatted   : atomic_formatted
| struct_formatted
| vector_formatted

atomic_formatted    : F_CHARCONST {$$ = cconstnode();}
| F_INTCONST  {$$ = iconstnode();}
| F_REALCONST {$$ = rconstnode();}
| F_STRCONST  {$$ = sconstnode();}
| F_BOOLCONST {$$ = bconstnode();}

struct_formatted :  '(' attr_list ')' {$$ = structnode(); $$->child = $2; $2->brother = $3;}

attr_list   : attr ',' attr_list {$$ = $1; $$->brother = $3;}
            | attr

attr : FORMATTED_LEX_ID {} formatted {}

vector_formatted : '(' formatted_list ')' {}

formatted_list   : formatted ',' formatted_list
| formatted




%%

int yyerror(){
    fprintf(stderr, "Error while parsing the format.\n");
    return -1;
}

int parse_format(char * format){
    int result;
    format_scan_string(format,strlen(format));
    if((result = formatparse()) == 0)
    print_sch(root);
    
    return 0;
}