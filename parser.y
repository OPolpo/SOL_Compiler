%{
#include "def.h"
#define YYSTYPE Pnode
extern char *yytext;
extern Value lexval;
extern int line;
extern FILE *yyin;
Pnode root = NULL;
%}

%token DEF INTEGER STRING BOOLEAN ID INTCONST STRCONST BOOLCONST ASSIGN
%token ERROR
%left AND OR
%nonassoc EQ NE '>' GE '<' LE IN
%left '+' '-'
%left '*' '/'
%right '-' NOT 

%%

program : func_decl
func_decl : func id '(' decl_list_opt ')' ':' domain type_sect_opt var_sect_opt const_sect_opt func_list_opt func_body
decl_list_opt : decl_list 
	| \** eps **\
decl_list : decl ';' decl_list 
	| decl ';'
decl : id_list ':' domain
id_list : id ',' id_list 
	| id
domain : atomic_domain 
	| struct_domain 
	| vector_domain 
	| id
atomic_domain : char 
	| int 
	| real 
	| string 
	| bool
struct_domain : struct '(' decl_list ')'
vector_domain : vector '[' intconst ']' of domain
type_sect_opt : type decl_list 
	| \** eps **\
var_sect_opt : var decl_list 
	| \** eps **\
const_sect_opt : const const_list 
	| \** eps **\
const_list : const_decl const_list 
	| const_decl
const_decl : decl = expr ';'
func_list_opt : func_list 
	| \** eps **\
func_list : func_decl func_list 
	| func_decl
func_body : begin id stat_list end id
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
assign_stat : left_hand_side = expr
left_hand_side : id 
	| fielding 
	| indexing
fielding : left_hand_side . id
indexing : left_hand_side '[' expr ']'
if_stat : if expr then stat_list elsif_stat_list_opt else_stat_opt endif
elsif_stat_list_opt : elsif expr then stat_list elsif_stat_list_opt 
	| \** eps **\
else_stat_opt : else stat_list 
	| \** eps **\
while_stat : while expr do stat_list endwhile
for_stat : for id = expr to expr do stat_list endfor
foreach_stat : foreach id in expr do stat_list endforeach
return_stat : return expr
read_stat : read specifier_opt id
specifier_opt : '[' expr ']' 
	| \** eps **\
write_stat : write specifier_opt expr
expr : expr bool_op bool_term 
	| bool_term
bool_op : and 
	| or
bool_term : rel_term rel_op rel_term 
	| rel_term
rel_op : == 
	| != 
	| > 
	| >= 
	| < 
	| <= 
	| in
rel_term : rel_term low_bin_op low_term 
	| low_term
low_bin_op : + 
	| −
low_term : low_term high_bin_op factor 
	| factor
high_bin_op : * 
	| /
factor : unary_op factor 
	| '(' expr ')' 
	| left_hand_side 
	| atomic_const 
	| instance_construction 
	| func_call 
	| cond_expr 
	| built_in_call 
	| dynamic_input
unary_op : '−' 
	| not 
	| dynamic_output
atomic_const : charconst 
	| intconst 
	| realconst 
	| strconst 
	| boolconst
instance_construction : struct_construction 
	| vector_construction
struct_construction : struct '(' expr_list ')'
expr_list : expr ',' expr_list 
	| expr
vector_construction : vector '(' expr_list ')'
func_call : id '(' expr_list_opt ')'
expr_list_opt : expr_list 
	| \** eps **\
cond_expr : if expr then expr elsif_expr_list_opt else expr endif
elsif_expr_list_opt : elsif expr then expr elsif_expr_list_opt 
	| \** eps **\ 
built_in_call : toint_call 
	| toreal_call
toint_call : toint '(' expr ')'
toreal_call : toreal '(' expr ')'
dynamic_input : rd specifier_opt domain
dynamic_output : wr specifier_opt

%%
