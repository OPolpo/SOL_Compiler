#include "def_y.h"
#include "parser.h"
#include "tree.h"
#include <string.h>

char* tabtypes[] =
{
    "ID",
    "ATOMIC_DOMAIN",
    "CHARCONST",
    "INTCONST",
    "REALCONST",
    "STRCONST",
    "BOOLCONST",
    "NONTERMINAL"
};

char* tabnonterm[] =
{
    "PROGRAM",
    "FUNC_DECL",
    "DECL_LIST_OPT",
    "DECL",
    "ID_LIST",
    "DOMAIN",
    "STRUCT_DOMAIN",
    "VECTOR_DOMAIN",
    "TYPE_SECT_OPT",
    "VAR_SECT_OPT",
    "CONST_SECT_OPT",
    "FUNC_LIST_OPT",
    "FUNC_BODY",
    "STAT_LIST",
    "STAT",
    "ASSIGN_STAT",
    "LEFT_HAND_SIDE",
    "FIELDING",
    "INDEXING",
    "IF_STAT",
    "ELSIF_STAT_LIS_OPT",
    "WHILE_STAT",
    "FOR_STAT",
    "FOREACH_STAT",
    "RETURN_STAT",
    "READ_STAT",
    "SPECIFIER_OPT",
    "WRITE_STAT",
    "COND_EXP",
    "ELSIF_EXPR_LIST_OPT",
    "BUILT_IN_CALL"
};

void treeprint(Pnode root, char* father_indent) {
    if(root==NULL) return;
    int i;
    Pnode p;

    char* my_indent = malloc(sizeof(char)*1000);
    my_indent[0]=0;
    strcpy(my_indent, father_indent);
    strcat(my_indent,"   |");
    printf("%s-%s", my_indent, (root->type == T_NONTERMINAL ? tabnonterm[root->value.ival] : tabtypes[root->type]));
    if(root->type == T_ID || root->type == T_STRCONST)
        printf(" (%s)", root->value.sval);
    else if(root->type == T_ATOMIC_DOMAIN)
        switch(root->value.ival){
            case CHAR:
                printf(" (CHAR)");
                break;
            case INT:
                printf(" (INT)");
                break;
            case REAL:
                printf(" (REAL)");
                break;
            case STRING:
                printf(" (STRING)");
                break;
            case BOOL:
                printf(" (BOOL)");
                break;
            default:
                printf("error %d", root->value.ival);
                break;
                
        }
    else if(root->type == T_INTCONST)
        printf(" (%d)", root->value.ival);
    else if(root->type == T_CHARCONST)
        printf(" (%c)", root->value.ival);
    else if(root->type == T_REALCONST)
        printf(" (%f)", root->value.rval);
    else if(root->type == T_BOOLCONST)
        printf(" (%s)", (root->value.bval == TRUE ? "TRUE" : "FALSE"));
    printf("\n");
    for(p=root->child; p != NULL; p = p->brother){
        if(root->brother == NULL){
            my_indent[strlen(my_indent)-4]=0;
            strcat(my_indent,"    ");
        }
        treeprint(p, my_indent);
    }
    free(my_indent);
}
