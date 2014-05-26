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

program : func_decl {root = $$ = nontermnode(NPROGRAM); $$->child = $1;}
func_decl : FUNC ID {$$=idnode();} '(' decl_list_opt ')' ':' domain type_sect_opt var_sect_opt const_sect_opt func_list_opt func_body {$$ = nontermnode(NFUNC_DECL); 
	/*		$1	 $2		$3			$4		$5		  $6 $7		$8		$9				$10			$11				$12			$13 */
																																	   $$->child = $3; 
																																	   $3->brother = $5;
																																	   $5->brother = $8;
																																	   $8->brother = $9;
																																	   $9->brother = $10;
																																	   $10->brother = $11;
																																	   $11->brother= $12;
																																	   $12->brother= $13;
																																   }
																								 
decl_list_opt : decl_list {$$ = nontermnode(NDECL_LIST_OPT); $$->child = $1;}
			  | /** eps **/ {$$ = nontermnode(NDECL_LIST_OPT);}
decl_list : decl ';' decl_list {$$->brother = $3;}
		  | decl ';'
decl : id_list ':' domain {$$ = nontermnode(NDECL);
						   $$->child = nontermnode(NID_LIST);
						   $$->child->child = $1;
						   $$->child->brother = $3;}
id_list : ID {$$ = idnode();} ',' id_list {$$ = $2;
										   $$->brother = $4;}
		| ID {$$ = idnode();}
domain : atomic_domain {$$ = nontermnode(NDOMAIN); $$->child = $1;}
	   | struct_domain {$$ = nontermnode(NDOMAIN); $$->child = $1;}
	   | vector_domain {$$ = nontermnode(NDOMAIN); $$->child = $1;}
	   | ID  {$$ = nontermnode(NDOMAIN); $$->child = idnode();}
atomic_domain : CHAR {$$ = atomicdomainnode(CHAR);}
			  | INT {$$ = atomicdomainnode(INT);}
			  | REAL {$$ = atomicdomainnode(REAL);}
			  | STRING {$$ = atomicdomainnode(STRING);}
			  | BOOL {$$ = atomicdomainnode(BOOL);}
struct_domain : STRUCT '(' decl_list ')' {$$ = nontermnode(NSTRUCT_DOMAIN);
										  $$->child = $3;}
vector_domain : VECTOR '[' INTCONST {$$ = iconstnode();} ']' OF domain {$$ = nontermnode(NVECTOR_DOMAIN);
																		$$->child = $4;
																		$$->child->brother = $7;}	   
type_sect_opt : TYPE decl_list {$$ = nontermnode(NTYPE_SECT_OPT);
								$$->child = $2;}
			  | /** eps **/{$$ = nontermnode(NTYPE_SECT_OPT);}
var_sect_opt : VAR decl_list  {$$ = nontermnode(NVAR_SECT_OPT);
							   $$->child = $2;}
			 | /** eps **/{$$ = nontermnode(NVAR_SECT_OPT);}


const_sect_opt : CONST const_list {$$ = nontermnode(NCONST_SECT_OPT);
							 	   $$->child = $2;}
			   | /** eps **/ {$$ = nontermnode(NCONST_SECT_OPT);}
const_list : const_decl const_list {$$->brother = $2;}
		   | const_decl
const_decl : decl '=' expr ';' {$$ = $1; $1->brother = $3;}
							  
							  
func_list_opt : func_list {$$ = nontermnode(NFUNC_LIST_OPT);
						   $$->child = $1;}
			  | /** eps **/ {$$ = nontermnode(NFUNC_LIST_OPT);}
func_list : func_decl func_list {$$ = $1; $$->brother = $2;}
		  | func_decl
func_body : F_BEGIN ID {$$ = idnode();} stat_list F_END ID {$$ = nontermnode(NFUNC_BODY);
										   					$$->child = $3;
										   				 	$3->brother = nontermnode(NSTAT_LIST);
										   				 	$3->brother->child = $4;
										   				 	$3->brother->brother = idnode();}
stat_list : stat ';' stat_list {$$ = $1; $$->brother = $3;}
	| stat ';' 
stat : assign_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| if_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| while_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| for_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| foreach_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| return_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| read_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
	| write_stat {$$ = nontermnode(NSTAT); $$->child = $1;}
assign_stat : left_hand_side '=' expr {$$ = nontermnode(NASSIGN_STAT); 
									   $$->child = $1; 
									   $1->brother = $3;}
left_hand_side : ID {$$ = nontermnode(NLEFT_HAND_SIDE); $$ = idnode();}
			   | fielding {$$ = nontermnode(NLEFT_HAND_SIDE); $$->child = $1;}
			   | indexing {$$ = nontermnode(NLEFT_HAND_SIDE); $$->child = $1;}
fielding : left_hand_side '.' ID {$$ = nontermnode(NFIELDING);
								  $$->child = $1;
								  $1->brother = idnode();}
indexing : left_hand_side '[' expr ']' {$$ = nontermnode(NINDEXING); 
								  		$$->child = $1;
								  	  	$1->brother = $3;}
if_stat : IF expr THEN stat_list elsif_stat_list_opt else_stat_opt ENDIF {$$ = nontermnode(NIF_STAT);
	/*	  $1  $2  $3	$4				$5				$6*/
																		  $$->child = $2;
																		  $2->brother = $4;
																		  $4->brother = $5;
																		  $5->brother = $6;
																	  }
elsif_stat_list_opt : ELSIF expr THEN stat_list elsif_stat_list_opt {$$ = nontermnode(NELSIF_STAT_LIS_OPT);
																	 $$->child = $2;
																	 $2->brother = $4;
																	 $4->brother = $5;}
	| /** eps **/ {$$ = nontermnode(NELSIF_STAT_LIS_OPT);}
else_stat_opt : ELSE stat_list {$$ = $2;}
	| /** eps **/
while_stat : WHILE expr DO stat_list ENDWHILE {$$ = nontermnode(NWHILE_STAT);
											   $$->child = $2;
											   $2->brother = $4;}
for_stat : FOR ID {$$ = idnode();} '=' expr TO expr DO stat_list ENDFOR {$$ = nontermnode(NFOR_STAT);
																		 $$->child = $3;
																		 $3->brother = $5;
																		 $5->brother = $7;
																		 $7->brother = $9;}
foreach_stat : FOREACH ID {$$ = idnode();} IN expr DO stat_list ENDFOREACH {$$ = nontermnode(NFOREACH_STAT);
														  	 				$$->child = $3;
														   				 	$3->brother = $5;
														   				 	$5->brother = $7;}
return_stat : RETURN expr {$$ = nontermnode(NRETURN_STAT);
						   $$->child = $2;}
read_stat : READ specifier_opt ID {$$ = nontermnode(NREAD_STAT);
						   		   $$->child = $2;
							   	   $2->brother = idnode();}
specifier_opt : '[' expr ']' {$$ = nontermnode(NSPECIFIER_OPT);
							  $$->child = $2;}
			  | /** eps **/ {$$ = nontermnode(NSPECIFIER_OPT);}
write_stat : WRITE specifier_opt expr {$$ = nontermnode(NWRITE_STAT);
						   		   	   $$->child = $2;
							   	       $2->brother = idnode();}
expr : expr bool_op bool_term {$$ = iconstnode();/*TO BE REMOVED*/}
	| bool_term {$$ = iconstnode();/*TO BE REMOVED*/}
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
		treeprint(root, "");
	return(result);
}

int yyerror(void){
  fprintf(stderr, "Line %d: syntax error on symbol \"%s\"\n", line, yytext);
  exit(-1);
}
