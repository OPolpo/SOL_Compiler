#include "s_machine.h"

Scode *prog;
int pc;

void exec(Scode *stat) {
    switch (stat->op) {
        case S_PUSH: exec_push(stat->args[0].ival, stat->args[1].ival, pc+1); break; case S_GOTO: exec_goto(stat->args[0].ival); break;
        case S_POP: exec_pop(); break;
        case S_NEW: exec_new(stat->args[0].ival); break;
        case S_NEWS: exec_news(stat->args[0].ival); break;
        case S_LDC: exec_ldc(stat->args[0].ival); break;
        case S_LDI: exec_ldi(stat->args[0].ival); break;
        case S_LDS: exec_lds(stat->args[0].sval); break;
        case S_LDR: exec_ldr(stat->args[0].rval); break;
            //TO DO
        case S_RETURN: exec_return(); break;
        default: machine_error("Unknown operator"); break; }
}

void exec_goto(int addr) {
    pc = addr;
}
void exec_jmp(int offset) {
    pc += offset-1;
}
void exec_jmf(int offset) {
    if(!pop_bool())
        pc += offset-1;
}
void exec_return() {
    pc = top_astack()->raddr;
}

void exec_iplus(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m+n);
}

void exec_igt(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m>n);
}

void exec_new(int size) {
    Odescr *po;
    po = push_ostack();
    po->mode = EMB;
    po->size = size;
}


