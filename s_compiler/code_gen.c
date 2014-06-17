#include "code_gen.h"
#include "parser.h"

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

Code make_lds(char *s){
    Code code;
    code = makecode(S_LDS);
    code.head->args[0].sval = s;
    return code;
}