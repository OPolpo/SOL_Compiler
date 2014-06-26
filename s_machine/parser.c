#include "s_machine.h"
#include "parser.h"

extern FILE * yyin;
extern int yylex();
extern char * yytext;
extern Scode * prog;
extern int pc;

void load_scode(FILE * input){
    yyin = input;
    int a;
    int total_instruction = 0;
    while(1){
        a = next();
        switch(a){
            case S_SCODE:
                next();
                total_instruction = atoi(yytext);
                prog = (Scode *)newmem(atoi(yytext));
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
            prog[pc].args[2].sval=insert_str_c(yytext);
            break;

            case S_PUSH:
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
            prog[pc].args[0].sval=insert_str_c(yytext);
            break;

            case S_LDR:
            prog[pc].op=a;
            next();
            prog[pc].args[0].rval=atof(yytext);
            break;
            case S_LDC:
            prog[pc].op=a;
            next();
            prog[pc].args[0].cval=yytext[0];
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
        
        pc++;
        if (pc>total_instruction)
            break;
    }
    printf("inizio stampa\n");
    print_loaded_code(prog);
    printf("fine stampa\n");

}

int next(){
    return yylex();
}

void print_loaded_code(Scode * prog){
    int i = 0;
    for(i=0;i<pc;i++){
        print_code_instruction(&prog[i]);
    }
}

void print_code_instruction(Scode * line){
    Stat to_print = {pc,line->op,{line->args[0],line->args[1],line->args[2]},0};
    print_stat(stdout, &to_print);
}