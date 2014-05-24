%{
#include "def_y.h"
#include "tree.h"

#define YYSTYPE Pnode
extern char *yytext;
extern Value lexval;
extern unsigned int line;
extern FILE *yyin;
Pnode root = NULL;

Pnode nontermnode(Nonterminal nonterm){
    Pnode p = newnode(T_NONTERMINAL);
    p->value.ival = nonterm;
    return(p);
}

Pnode idnode(){
    Pnode p = newnode(T_ID);
    p->value.sval = lexval.sval;
    return(p);
}

Pnode iconstnode(){
    Pnode p = newnode(T_INTCONST);
    p->value.ival = lexval.ival;
    return(p);
}

Pnode cconstnode(){
    Pnode p = newnode(T_CHARCONST);
    p->value.ival = lexval.ival;
    return(p);
}

Pnode rconstnode(){
    Pnode p = newnode(T_REALCONST);
    p->value.rval = lexval.rval;
    return(p);
}

Pnode sconstnode(){
    Pnode p = newnode(T_STRCONST);
    p->value.sval = lexval.sval;
    return(p);
}

Pnode bconstnode(){
  Pnode p = newnode(T_BOOLCONST);
  p->value.bval = lexval.bval;
  return(p);
}

Pnode atomicdomainnode(int domain){
  Pnode p = newnode(T_ATOMIC_DOMAIN);
  p->value.ival = domain;
  return(p);
}

Pnode newnode(Typenode tnode){
  Pnode p = malloc(sizeof(Node));
  p->type = tnode;
  p->child = p->brother = NULL;
  return(p);
}
%}

%token CHAR	
 INT REAL STRING BOOL STRUCT VECTOR OF TYPE VAR CONST FUNC TOINT TOREAL F_BEGIN F_END REL_OP EQ NE LT LE GT GE IN LOG_OP AND OR NOT IF THEN ELSE ELSIF ENDIF WHILE DO ENDWHILE FOR TO ENDFOR FOREACH ENDFOREACH RETURN READ RD WRITE WR ASSIGN ERROR INTCONST REALCONST CHARCONST STRCONST BOOLCONST ID

%left AND OR
%nonassoc EQ NE '>' GE '<' LE IN
%left '+' '-'
%left '*' '/'
%right UMINUS NOT 

%%

program : func_decl {root = $$ = nontermnode(NPROGRAM); $$->child = $1}
func_decl : FUNC ID {$$=idnode();} '(' decl_list_opt ')' ':' domain type_sect_opt var_sect_opt const_sect_opt func_list_opt func_body {$$ = nontermnode(NFUNC_DECL); 
																																	   $$->child = $3; 
																																	   $3->brother = $5;}
decl_list_opt : decl_list {$$ = nontermnode(NDECL_LIST_OPT);
						   $$->child = $1}
			  | /** eps **/
decl_list : decl ';' decl_list {$$->brother = $3}
		  | decl ';'
decl : id_list ':' domain {$$ = nontermnode(NDECL);
						   $$->child = nontermnode(NID_LIST);
						   $$->child->child = $1;
						   $$->child->brother = $3;}
id_list : ID {$$ = idnode();} ',' id_list {$$ = $2;
										  $2->brother = $4;}
		| ID {$$ = idnode();}
domain : atomic_domain 
	   | struct_domain 
	   | vector_domain 
	   | ID
atomic_domain : CHAR {$$ = atomicdomainnode(CHAR);}
			  | INT {$$ = atomicdomainnode(INT);}
			  | REAL {$$ = atomicdomainnode(REAL);}
			  | STRING {$$ = atomicdomainnode(STRING);}
			  | BOOL {$$ = atomicdomainnode(BOOL);}


struct_domain : STRUCT '(' decl_list ')'
vector_domain : VECTOR '[' INTCONST ']' OF domain
type_sect_opt : TYPE decl_list 
	| /** eps **/
var_sect_opt : VAR decl_list 
	| /** eps **/
const_sect_opt : CONST const_list 
	| /** eps **/
const_list : const_decl const_list 
	| const_decl
const_decl : decl '=' expr ';'
func_list_opt : func_list 
	| /** eps **/
func_list : func_decl func_list 
	| func_decl
func_body : F_BEGIN ID stat_list F_END ID
stat_list : stat ';' stat_list 
	| stat ';'
stat : assign_stat 
	| if_stat 
	| while_stat 
	| for_stat 
	| foreach_stat 
	| return_stat 
	| read_stat 
	| write_stat
assign_stat : left_hand_side '=' expr
left_hand_side : ID 
	| fielding 
	| indexing
fielding : left_hand_side '.' ID
indexing : left_hand_side '[' expr ']'
if_stat : IF expr THEN stat_list elsif_stat_list_opt else_stat_opt ENDIF
elsif_stat_list_opt : ELSIF expr THEN stat_list elsif_stat_list_opt 
	| /** eps **/
else_stat_opt : ELSE stat_list 
	| /** eps **/
while_stat : WHILE expr DO stat_list ENDWHILE
for_stat : FOR ID '=' expr TO expr DO stat_list ENDFOR
foreach_stat : FOREACH ID IN expr DO stat_list ENDFOREACH
return_stat : RETURN expr
read_stat : READ specifier_opt ID
specifier_opt : '[' expr ']' 
	| /** eps **/
write_stat : WRITE specifier_opt expr
expr : expr bool_op bool_term 
	| bool_term
bool_op : AND 
	| OR
bool_term : rel_term rel_op rel_term 
	| rel_term
rel_op : EQ
	| NE 
	| '>' 
	| GE 
	| '<' 
	| LE 
	| IN
rel_term : rel_term low_bin_op low_term 
	| low_term
low_bin_op : '+' 
	| '-'
low_term : low_term high_bin_op factor 
	| factor
high_bin_op : '*' 
	| '/'
factor : unary_op factor 
	| '(' expr ')' 
	| left_hand_side 
	| atomic_const 
	| instance_construction 
	| func_call 
	| cond_expr 
	| built_in_call 
	| dynamic_input
unary_op : '−' %prec UMINUS
	| NOT 
	| dynamic_output
atomic_const : CHARCONST 
	| INTCONST 
	| REALCONST 
	| STRCONST 
	| BOOLCONST
instance_construction : struct_construction 
	| vector_construction
struct_construction : STRUCT '(' expr_list ')'
expr_list : expr ',' expr_list 
	| expr
vector_construction : VECTOR '(' expr_list ')'
func_call : ID '(' expr_list_opt ')'
expr_list_opt : expr_list 
	| /** eps **/
cond_expr : IF expr THEN expr elsif_expr_list_opt ELSE expr ENDIF
elsif_expr_list_opt : ELSIF expr THEN expr elsif_expr_list_opt 
	| /** eps **/ 
built_in_call : toint_call 
	| toreal_call
toint_call : TOINT '(' expr ')'
toreal_call : TOREAL '(' expr ')'
dynamic_input : RD specifier_opt domain
dynamic_output : WR specifier_opt

%%




int main(){
	int result;
	  
	yyin = stdin;
	if((result = yyparse()) == 0)
		treeprint(root, 0);
	return(result);
}

int yyerror(void){
  fprintf(stderr, "Line %d: syntax error on symbol \"%s\"\n", line, yytext);
  exit(-1);
}
