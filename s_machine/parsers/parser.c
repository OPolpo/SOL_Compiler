/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief S-code loader
 */


#include "../s_machine.h"
#include "parser.h"

extern FILE * yyin;
extern int yylex();
extern char * yytext;
extern Scode * prog;
extern int pc;
extern int code_size;

/**
 * @brief This function initializes the code memory.
 * @param input file pointer to the s_code file
 */
void load_scode(FILE * input){
    yyin = input;
    int a;
    while(1){
        a = next();
        switch(a){
            case S_SCODE:
                next();
                code_size = atoi(yytext);
                prog = (Scode *)newmem(code_size * sizeof(Scode));
                pc--;
                break;
                
            case S_HALT:
            case S_TOINT:
            case S_TOREAL:
            case S_POP:
            case S_IUMI:
            case S_RUMI:
            case S_NEG:
            case S_IN:
            case S_IPLUS:
            case S_RPLUS:
            case S_IMINUS:
            case S_RMINUS:
            case S_ITIMES:
            case S_RTIMES:
            case S_IDIV:
            case S_RDIV:
            case S_CGT:
            case S_IGT:
            case S_RGT:
            case S_SGT:
            case S_CGE:
            case S_IGE:
            case S_RGE:
            case S_SGE:
            case S_CLT:
            case S_ILT:
            case S_RLT:
            case S_SLT:
            case S_CLE:
            case S_ILE:
            case S_RLE:
            case S_SLE:
            case S_EQU:
            case S_NEQ:
            case S_IST:
            case S_RETURN:
                prog[pc].op=a;
                break;
                
            case S_READ:
            case S_FREAD:
                prog[pc].op=a;
                next();
                prog[pc].args[0].ival=atoi(yytext);
                next();
                prog[pc].args[1].ival=atoi(yytext);
                next();
                prog[pc].args[2].sval=insert_strWclean(yytext);
                break;
                
            case S_PUSH:
                prog[pc].op=a;
                next();
                prog[pc].args[0].ival=atoi(yytext);
                next();
                prog[pc].args[1].ival=atoi(yytext);
                next();
                prog[pc].args[2].ival=atoi(yytext);
                break;
                
            case S_STO:
            case S_LDA:
            case S_CAT:
            case S_LOD:
                prog[pc].op=a;
                next();
                prog[pc].args[0].ival=atoi(yytext);
                next();
                prog[pc].args[1].ival=atoi(yytext);
                break;
                
            case S_WRITE:
            case S_FWRITE:
            case S_RD:
            case S_FRD:
            case S_WR:
            case S_FWR:
            case S_LDS:
                prog[pc].op=a;
                next();
                prog[pc].args[0].sval=insert_strWclean(yytext);
                break;
                
            case S_LDR:
                prog[pc].op=a;
                next();
                prog[pc].args[0].rval=(float)atof(yytext);
                break;
            case S_LDC:
                prog[pc].op=a;
                next();
                prog[pc].args[0].cval=yytext[1];
                break;
                
            case S_FUNC:
            case S_GOTO:
            case S_FAKE_GOTO:
            case S_JMF:
            case S_JMP:
            case S_IXA:
            case S_EIL:
            case S_SIL:
            case S_FDA:
            case S_LDI:
            case S_NEW:
            case S_NEWS:
                prog[pc].op=a;
                next();
                prog[pc].args[0].ival=atoi(yytext);
                break;
            default:
                fprintf(stderr, "ERROR\n");
        }
        
        
        if (pc==code_size-1)
            break;
        pc++;
    }
}

/**
 * @brief This function ask to the lexer the next token.
 * @return the next lexer recognized token
 */
int next(){
    return yylex();
}
