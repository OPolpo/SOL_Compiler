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
        case S_LOD: exce_lod(stat->args[0].ival); break;
        case S_CAT: exce_cat(stat->args[0].ival); break;
        case S_LDA: exce_lda(stat->args[0].ival); break;
        case S_FDA: exce_fda(stat->args[0].ival); break;
        case S_EIL: exce_eil(stat->args[0].ival); break;
        case S_SIL: exce_sil(stat->args[0].ival); break;
        case S_IXA: exce_ixa(stat->args[0].ival); break;
        case S_STO: exce_sto(stat->args[0].ival); break;
        case S_IST: exce_ist(stat->args[0].ival); break;
        case S_JMF: exce_jmf(stat->args[0].ival); break;
        case S_JMP: exce_jmp(stat->args[0].ival); break;
        case S_EQU: exce_equ(stat->args[0].ival); break;
        case S_NEQ: exce_neq(stat->args[0].ival); break;
        case S_CGT: exce_cgt(stat->args[0].ival); break;
        case S_IGT: exce_igt(stat->args[0].ival); break;
        case S_RGT: exce_rgt(stat->args[0].ival); break;
        case S_SGT: exce_sgt(stat->args[0].ival); break;
        case S_CGE: exce_cge(stat->args[0].ival); break;
        case S_IGE: exce_ige(stat->args[0].ival); break;
        case S_RGE: exce_rge(stat->args[0].ival); break;
        case S_SGE: exce_sge(stat->args[0].ival); break;
        case S_CLT: exce_clt(stat->args[0].ival); break;
        case S_ILT: exce_ilt(stat->args[0].ival); break;
        case S_RLT: exce_rlt(stat->args[0].ival); break;
        case S_SLT: exce_slt(stat->args[0].ival); break;
        case S_CLE: exce_cle(stat->args[0].ival); break;
        case S_ILE: exce_ile(stat->args[0].ival); break;
        case S_RLE: exce_rle(stat->args[0].ival); break;
        case S_SLE: exce_sle(stat->args[0].ival); break;
        case S_IN: exce_in(stat->args[0].ival); break;
        case S_IPLUS: exce_iplus(stat->args[0].ival); break;
        case S_RPLUS: exce_rplus(stat->args[0].ival); break;
        case S_IMINUS: exce_iminus(stat->args[0].ival); break;
        case S_RMINUS: exce_rminus(stat->args[0].ival); break;
        case S_ITIMES: exce_itimes(stat->args[0].ival); break;
        case S_RTIMES: exce_rtimes(stat->args[0].ival); break;
        case S_IDIV: exce_idiv(stat->args[0].ival); break;
        case S_RDIV: exce_rdiv(stat->args[0].ival); break;
        case S_IUMI: exce_iumi(stat->args[0].ival); break;
        case S_RUMI: exce_rumi(stat->args[0].ival); break;
        case S_NEG: exce_neg(stat->args[0].ival); break;
        case S_WR: exce_wr(stat->args[0].ival); break;
        case S_FWR: exce_fwr(stat->args[0].ival); break;
        case S_PUSH: exce_push(stat->args[0].ival); break;
        case S_GOTO: exce_goto(stat->args[0].ival); break;
        case S_POP: exce_pop(stat->args[0].ival); break;
        case S_RD: exce_Rd(stat->args[0].ival); break;
        case S_FRD: exce_frd(stat->args[0].ival); break;
        case S_TOINT: exce_toin(stat->args[0].ival); break;
        case S_TOREAL: exce_toreal(stat->args[0].ival); break;
        case S_READ: exce_read(stat->args[0].ival); break;
        case S_FREAD: exce_fread(stat->args[0].ival); break;
        case S_WRITE: exce_write(stat->args[0].ival); break;
        case S_FWRITE: exce_fwrite(stat->args[0].ival); break;
        case S_FUNC: exce_func(stat->args[0].ival); break;
        case S_HALT: exce_halt(stat->args[0].ival); break;
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

void exec_news(stat->args[0].ival){

}
void exec_ldc(stat->args[0].ival){

}
void exec_ldi(stat->args[0].ival){

}
void exec_lds(stat->args[0].sval){

}
void exec_ldr(stat->args[0].rval){

}
void exce_lod(stat->args[0].ival){

}
void exce_cat(stat->args[0].ival){

}
void exce_lda(stat->args[0].ival){

}
void exce_fda(stat->args[0].ival){

}
void exce_eil(stat->args[0].ival){

}
void exce_sil(stat->args[0].ival){

}
void exce_ixa(stat->args[0].ival){

}
void exce_sto(stat->args[0].ival){

}
void exce_ist(stat->args[0].ival){

}
void exce_equ(stat->args[0].ival){

}
void exce_neq(stat->args[0].ival){

}
void exce_cgt(stat->args[0].ival){

}
void exce_rgt(stat->args[0].ival){

}
void exce_sgt(stat->args[0].ival){

}
void exce_cge(stat->args[0].ival){

}
void exce_ige(stat->args[0].ival){

}
void exce_rge(stat->args[0].ival){

}
void exce_sge(stat->args[0].ival){

}
void exce_clt(stat->args[0].ival){

}
void exce_ilt(stat->args[0].ival){

}
void exce_rlt(stat->args[0].ival){

}
void exce_slt(stat->args[0].ival){

}
void exce_cle(stat->args[0].ival){

}
void exce_ile(stat->args[0].ival){

}
void exce_rle(stat->args[0].ival){

}
void exce_sle(stat->args[0].ival){

}
void exce_in(stat->args[0].ival){

}
void exce_rplus(stat->args[0].ival){

}
void exce_iminus(stat->args[0].ival){

}
void exce_rminus(stat->args[0].ival){

}
void exce_itimes(stat->args[0].ival){

}
void exce_rtimes(stat->args[0].ival){

}
void exce_idiv(stat->args[0].ival){

}
void exce_rdiv(stat->args[0].ival){

}
void exce_iumi(stat->args[0].ival){

}
void exce_rumi(stat->args[0].ival){

}
void exce_neg(stat->args[0].ival){

}
void exce_wr(stat->args[0].ival){

}
void exce_fwr(stat->args[0].ival){

}
void exce_push(stat->args[0].ival){

}
void exce_pop(stat->args[0].ival){

}
void exce_Rd(stat->args[0].ival){

}
void exce_frd(stat->args[0].ival){

}
void exce_toin(stat->args[0].ival){

}
void exce_toreal(stat->args[0].ival){

}
void exce_read(stat->args[0].ival){

}
void exce_fread(stat->args[0].ival){

}
void exce_write(stat->args[0].ival){

}
void exce_fwrite(stat->args[0].ival){

}
void exce_func(stat->args[0].ival){

}
void exce_halt(stat->args[0].ival){

}


