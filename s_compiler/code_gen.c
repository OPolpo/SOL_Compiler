/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief This file contains the function to manage the code generation data structures.
 */


#include "code_gen.h"

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
    "EIL",//10
    "SIL",
    "IXA",
    "STO",
    "IST",
    "JMF",
    "JMP",
    "EQU",
    "NEQ",
    "CGT",
    "IGT",//20
    "RGT",
    "SGT",
    "CGE",
    "IGE",
    "RGE",
    "SGE",
    "CLT",
    "ILT",
    "RLT",
    "SLT",//30
    "CLE",
    "ILE",
    "RLE",
    "SLE",
    "IN",
    "IPLUS",
    "RPLUS",
    "IMINUS",
    "RMINUS",
    "ITIMES",//40
    "RTIMES",
    "IDIV",
    "RDIV",
    "IUMI",
    "RUMI",
    "NEG",
    "WR",
    "FWR",
    "PUSH",
    "GOTO",//50
    "POP",
    "RD",
    "FRD",
    "TOINT",
    "TOREAL",
    "READ",
    "FREAD",
    "WRITE",
    "FWRITE",
    "FUNC",//60
    "HALT",
    "SCODE",
    "RETURN",
    "FAKE_RETURN",
    "---",
    "FAKE_GOTO",
    "FAKE_PUSH"
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
    if (code1.head == code2.head && code1.tail == code2.tail) {
        printf("Trying to concatenate a list with itself <==============\n");
        return code1;
    }
    
    if (code1.head->op == S_NOOP ) {
        if (code1.tail != code1.head) {
            fprintf(stderr, "********* Strange code starting with NOOP\n");
        }
        return code2;
    }
    else if (code2.head->op == S_NOOP) {
        if (code2.tail != code2.head) {
            fprintf(stderr, "********* Strange code starting with NOOP\n");
        }
        return code1;
    }
    else{
        relocate_address(code2, code1.size);
        rescode.head = code1.head;
        rescode.tail = code2.tail;
        code1.tail->next = code2.head;
        rescode.size = code1.size + code2.size;
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

/*
 case S_PUSH
 case S_FAKE_PUSH */
Code makecode_fake_push(int arg1, int arg2, int arg3){
    Code code;
    code = makecode(S_FAKE_PUSH);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    code.head->args[2].ival = arg3;
    return code;
}

Code make_push_pop(int param, int oid, int chain, int entry){
    return concode(makecode_fake_push(param, oid, chain),
                   makecode1(S_FAKE_GOTO, entry),
                   makecode(S_POP),
                   endcode());
}

Code make_ldc(char c){
    Code code;
    code = makecode(S_LDC);
    code.head->args[0].cval = c;
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
        case S_FAKE_RETURN:
        case S_NOOP:
            break;
            
        case S_READ:
        case S_FREAD:
            fprintf(stream, "%d %d %s", stat->args[0].ival, stat->args[1].ival, stat->args[2].sval);
            break;
            
        case S_FAKE_PUSH:
        case S_PUSH:
            fprintf(stream, "%d %d %d", stat->args[0].ival, stat->args[1].ival, stat->args[2].ival);
            break;
            
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
            fprintf(stream, "'%c' ", stat->args[0].cval);
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

//Code Stack
void StackPush(Stack_node_code ** top, Code * element){
    Stack_node_code * new_node = (Stack_node_code *)malloc(sizeof(Stack_node_code));
    new_node->code = element;
    new_node->next = *top;
    *top = new_node;
}

Code * StackPop(Stack_node_code ** top){
    Code * element = NULL;
    if((*top)!=NULL){
        element = (*top)->code;
        (*top)=(*top)->next;
    }else{
        fprintf(stderr, "Stack Underflow\n");
    }
    return element;
}

Poid2address * new_o2a_table(){
    Poid2address * table = calloc(TOT, sizeof(Poid2address));
    return table;
}

int insert_o2a(Poid2address p, Poid2address * table){
    int pos = p->oid % TOT;
    p->next = table[pos];
    table[pos] = p;
    return 1;
}

int get_f_addr_by_oid(int oid, Poid2address * table){
    int pos = oid % TOT;
    Poid2address node = table[pos];
    while(node){
        if (node->oid == oid) {
            return *(node->address);
        }
        node = node->next;
    }
    return -1;
}

int get_f_num_obj_by_oid(int oid, Poid2address * table){
    int pos = oid % TOT;
    Poid2address node = table[pos];
    while(node){
        if (node->oid == oid) {
            return node->num_obj;
        }
        node = node->next;
    }
    return -1;
}

Poid2address new_o2a(int oid, int *addr){
    Poid2address res = calloc(1, sizeof(Oid2address));
    res->address = addr;
    res->oid = oid;
    return res;
}

void destroy_o2a(Poid2address * table){
    int i=0;
    for (i=0; i<TOT; i++) {
        if (table[i]!= NULL) {
            Poid2address newhead = table[i]->next;
            while(newhead){
                free(table[i]);//free old head
                table[i] = newhead;
                newhead=newhead->next;
            }
            free(table[i]);
        }
    }
}

void destroy_code(Code *code){
    Stat * to_free = code->head;
    Stat * newhead = to_free->next;
    while(newhead){
        free(to_free);//free old head
        to_free = newhead;
        newhead=newhead->next;
    }
    free(to_free);
}

