#include "code_gen.h"
//#include "parser.h"

char * tabOperator[]={
    "S_NEW",
    "S_NEWS",
    "S_LDC",
    "S_LDI",
    "S_LDR",
    "S_LDS",
    "S_LOD",
    "S_CAT",
    "S_LDA",
    "S_FDA",
    "S_EIL",
    "S_SIL",
    "S_IXA",
    "S_STO",
    "S_IST",
    "S_JMF",
    "S_JMP",
    "S_EQU",
    "S_NEQ",
    "S_CGT",
    "S_IGT",
    "S_RGT",
    "S_SGT",
    "S_CGE",
    "S_IGE",
    "S_RGE",
    "S_SGE",
    "S_CLT",
    "S_ILT",
    "S_RLT",
    "S_SLT",
    "S_CLE",
    "S_ILE",
    "S_RLE",
    "S_SLE",
    "S_IN",
    "S_IPLUS",
    "S_RPLUS",
    "S_IMINUS",
    "S_RMINUS",
    "S_ITIMES",
    "S_RTIMES",
    "S_IDIV",
    "S_RDIV",
    "S_IUMI",
    "S_RUMI",
    "S_NEG",
    "S_WR",
    "S_FWR",
    "S_PUSH",
    "S_GOTO",
    "S_POP",
    "S_RD",
    "S_FRD",
    "S_TOINT",
    "S_TOREAL",
    "S_READ",
    "S_FREAD",
    "S_WRITE",
    "S_FWRITE",
    "S_FUNC",
    "S_HALT",
    "S_SCODE"};

char convert_bool[] = {'0','1'};

void relocate_address(Code code, int offset){
    Stat *pt = code.head; int i;
    for(i = 1; i <= code.size; i++){
        pt->address += offset;
        pt = pt->next;
    }
}

Code appcode(Code code1, Code code2){
    Code rescode;
    relocate_address(code2, code1.size);
    rescode.head = code1.head;
    rescode.tail = code2.tail;
    code1.tail->next = code2.head;
    rescode.size = code1.size + code2.size;
    return rescode;
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
    pstat = (Stat*) malloc(sizeof(Stat));
    if (!pstat) {
        fprintf(stderr, "Impossible to allocate new memory\n");
        exit(EXIT_FAILURE);
    }
    pstat->address = 0;
    pstat->op = op;
    pstat->next = NULL;
    return pstat;
}

Code makecode(Operator op){
    Code code;
    code.head = code.tail = newstat(op);
    code.size = 1;
    return code;
}

Code makecode1(Operator op, int arg){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg;
    return code;
}

Code makecode2(Operator op, int arg1, int arg2){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    return code;
}

Code makecode3(Operator op, int arg1, int arg2, int arg3){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    code.head->args[2].ival = arg3;
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
    code.head->args[0].ival = c;
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

void print_stat(Stat * stat){
    fprintf(stdout, "%s ", tabOperator[stat->op]);
    fprintf(stdout, "\n");
    // if(stat->args[])
    // fprintf(stream, "%s ", tabOperator[stat->op]);
}

void print_code(Code * code){
    Stat * stat;
    stat = code->head;
    while(stat){
        print_stat(stat);
        stat = stat->next;
    }
}