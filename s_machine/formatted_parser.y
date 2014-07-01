%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "s_machine.h"
    #include "sol_io.h"
    
    #define YYSTYPE Pformatted
    
    extern Value lexval;
    extern Str_c_node ** format_stringtable;
    Pformatted formatted_root = NULL;
    
    Pformatted newnode(Typeformatted tnode){
        Pformatted p = malloc(sizeof(Formatted));
        p->type = tnode;
        p->child = p->brother = NULL;
        p->id = NULL;
        return(p);
    }
    
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
    

    
    
    %}
%token FORMATTED_LEX_ID CHARCONST INTCONST REALCONST STRCONST BOOLCONST FORMATTED_LEX_ERROR
%%

formatted_string : formatted {formatted_root = $$; $$ = $1;}

formatted   : atomic_formatted
            | struct_formatted
            | vector_formatted

atomic_formatted    : CHARCONST {$$ = cconstnode();}
                    | INTCONST  {$$ = iconstnode();}
                    | REALCONST {$$ = rconstnode();}
                    | STRCONST  {$$ = sconstnode();}
                    | BOOLCONST {$$ = bconstnode();}

struct_formatted :  '(' attr_list ')' {$$ = structnode(); $$->child = $2;}

attr_list   : attr ',' attr_list {$$ = $1; $$->brother = $3;}
            | attr

attr : FORMATTED_LEX_ID {$$ = newnode(F_TEMP); $$->id = lexval.sval;} formatted {$$ = $3; $3->id = $2->id; free($2);}

vector_formatted : '(' formatted_list ')' {$$ = vectornode(); $$->child = $2;}

formatted_list  : formatted ',' formatted_list {$$ = $1; $1->brother = $3;}
                | formatted


%%

int yyerror(){
    fprintf(stderr, "Error while parsing the format.\n");
    return -1;
}

int parse_formatted(char * formatted){
    int result;
    formatted_scan_string(formatted,strlen(formatted));
    if((result = formattedparse()) == 0)
    //print_sch(formatted_root);
    
    return 0;
}

