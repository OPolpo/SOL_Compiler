#include "code_gen.h"
//#include "parser.h"

char * tabOperator[]={
    "NEW",
    "NEWS",
    "LDC",
    "LDI",
    "LDR",
    "LDS",
    "LOD",
    "CAT",
    "LDA",
    "FDA",
    "EIL",
    "SIL",
    "IXA",
    "STO",
    "IST",
    "JMF",
    "JMP",
    "EQU",
    "NEQ",
    "CGT",
    "IGT",
    "RGT",
    "SGT",
    "CGE",
    "IGE",
    "RGE",
    "SGE",
    "CLT",
    "ILT",
    "RLT",
    "SLT",
    "CLE",
    "ILE",
    "RLE",
    "SLE",
    "IN",
    "IPLUS",
    "RPLUS",
    "IMINUS",
    "RMINUS",
    "ITIMES",
    "RTIMES",
    "IDIV",
    "RDIV",
    "IUMI",
    "RUMI",
    "NEG",
    "WR",
    "FWR",
    "PUSH",
    "GOTO",
    "POP",
    "RD",
    "FRD",
    "TOINT",
    "TOREAL",
    "READ",
    "FREAD",
    "WRITE",
    "FWRITE",
    "FUNC",
    "HALT",
    "SCODE",
    "RETURN",
    "---"
};


void relocate_address(Code code, int offset){
    Stat *pt = code.head; int i;
    for(i = 1; i <= code.size; i++){
        pt->address += offset;
        pt = pt->next;
    }
}

Code appcode(Code code1, Code code2){
    Code rescode;
    /*
     printf("********\nAppendo a: \n");
     print_code(stdout, &code1);
     printf("\nQUESTO: \n");
     print_code(stdout, &code2);
     printf("\nOttengo: \n");
     */
    
    if (code1.head->op == S_NOOP ) {
        if (code1.tail != code1.head) {
            fprintf(stderr, "********* Strange code starting with NOOP\n");
        }
        //print_code(stdout, &code2);
        //printf("********\nFINE append. \n");
        return code2;
    }
    else if (code2.head->op == S_NOOP) {
        if (code2.tail != code2.head) {
            fprintf(stderr, "********* Strange code starting with NOOP\n");
        }
        //print_code(stdout, &code2);
        //printf("********\nFINE append. \n");
        return code1;
    }
    else{
        relocate_address(code2, code1.size);
        rescode.head = code1.head;
        rescode.tail = code2.tail;
        code1.tail->next = code2.head;
        rescode.size = code1.size + code2.size;
        
        //print_code(stdout, &rescode);
        //printf("********\nFINE append. \n");
        
        return rescode;
    }
}

Code endcode(){
    static Code code = {NULL, 0, NULL};
    return code;
}

Code concode(Code code1, Code code2, ...){
    Code rescode = code1, *pcode = &code2;
    while(pcode->head != NULL){
        rescode = appcode(rescode, *pcode);
        pcode++;
    }
    return rescode;
}

Stat * newstat(Operator op){
    Stat *pstat;
    pstat = (Stat*) calloc(1,sizeof(Stat));
    if (!pstat) {
        fprintf(stderr, "Impossible to allocate new memory\n");
        exit(EXIT_FAILURE);
    }
    pstat->address = 0;
    pstat->op = op;
    pstat->next = NULL;
    return pstat;
}

/*
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
 case S_NOOP*/
Code makecode(Operator op){//for all codes without arguments
    Code code;
    code.head = code.tail = newstat(op);
    code.size = 1;
    return code;
}

/*
 case S_FUNC:
 case S_GOTO:
 case S_JMF:
 case S_JMP:
 case S_IXA:
 case S_EIL:
 case S_SIL:
 case S_FDA:
 case S_LDI:
 case S_NEW:
 case S_NEWS:
 case S_SCODE:*/
Code makecode1(Operator op, int arg){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg;
    return code;
}

/*
 case S_WRITE:
 case S_FWRITE:
 case S_RD:
 case S_FRD:
 case S_WR:
 case S_FWR:*/
Code makecode_str(Operator op, char * s){
    Code code;
    code = makecode(op);
    code.head->args[0].sval = s;
    return code;
}

/*
 case S_PUSH:
 case S_STO:
 case S_LDA:
 case S_CAT:
 case S_LOD:*/
Code makecode2(Operator op, int arg1, int arg2){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    return code;
}

/*
 case S_READ:
 case S_FREAD:*/
Code makecode_xread(Operator op, int arg1, int arg2, char * arg3){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    code.head->args[2].sval = arg3;
    return code;
}

Code make_push_pop(int size, int chain, int entry){
    return concode(makecode2(S_PUSH, size, chain),
                   makecode1(S_GOTO, entry),
                   makecode(S_POP),
                   endcode());
}

Code make_ldc(char c){
    Code code;
    code = makecode(S_LDC);
    code.head->args[0].bval = c;
    printf("in make_ldc '%c'\n", code.head->args[0].bval);
    return code;
}

Code make_ldi(int i){
    Code code;
    code = makecode(S_LDI);
    code.head->args[0].ival = i;
    return code;
}

Code make_ldr(float r){
    Code code;
    code = makecode(S_LDR);
    code.head->args[0].rval = r;
    return code;
}

Code make_lds(char *s){
    Code code;
    code = makecode(S_LDS);
    code.head->args[0].sval = s;
    return code;
}

void print_stat(FILE * stream, Stat * stat){
    fprintf(stream, "%s ", tabOperator[stat->op]);
    
    switch(stat->op){
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
        case S_NOOP:
            break;
            
        case S_READ:
        case S_FREAD:
            fprintf(stream, "%d %d %s", stat->args[0].ival, stat->args[1].ival, stat->args[2].sval);
            break;
            
        case S_PUSH:
        case S_STO:
        case S_LDA:
        case S_CAT:
        case S_LOD:
            fprintf(stream, "%d %d ", stat->args[0].ival, stat->args[1].ival);
            break;
            
        case S_WRITE:
        case S_FWRITE:
        case S_RD:
        case S_FRD:
        case S_WR:
        case S_FWR:
        case S_LDS:
            fprintf(stream, "%s ", stat->args[0].sval);
            break;
            
        case S_LDR:
            fprintf(stream, "%f ", stat->args[0].rval);
            break;
        case S_LDC:
            fprintf(stream, "'%c' ", stat->args[0].bval);
            break;
            
        case S_FUNC:
        case S_GOTO:
        case S_JMF:
        case S_JMP:
        case S_IXA:
        case S_EIL:
        case S_SIL:
        case S_FDA:
        case S_LDI:
        case S_NEW:
        case S_NEWS:
        case S_SCODE:
            fprintf(stream, "%d ", stat->args[0].ival);
            break;
        default:
            fprintf(stream, "ERROR");
    }
    fprintf(stream, "\n");
}

void print_code(FILE * stream, Code * code){
    Stat * stat;
    stat = code->head;
    while(stat){
        print_stat(stream, stat);
        stat = stat->next;
    }
}
