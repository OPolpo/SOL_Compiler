#include "s_exec.h"

Scode *prog;
int pc;

void exec(Scode *stat) {
    switch (stat->op) {
        case S_PUSH: exec_push(stat->args[0].ival, stat->args[1].ival, pc+1); break;
        case S_GOTO: exec_goto(stat->args[0].ival); break;
        case S_POP: exec_pop(); break;
        case S_NEW: exec_new(stat->args[0].ival); break;
        case S_NEWS: exec_news(stat->args[0].ival); break;
        case S_LDC: exec_ldc(stat->args[0].ival); break;
        case S_LDI: exec_ldi(stat->args[0].ival); break;
        case S_LDS: exec_lds(stat->args[0].sval); break;
        case S_LDR: exec_ldr(stat->args[0].rval); break;
        case S_LOD: exec_lod(stat->args[0].ival, stat->args[1].ival); break;
        case S_CAT: exec_cat(stat->args[0].ival, stat->args[1].ival); break;
        case S_LDA: exec_lda(stat->args[0].ival, stat->args[1].ival); break;
        case S_FDA: exec_fda(stat->args[0].ival); break;
        case S_EIL: exec_eil(stat->args[0].ival); break;
        case S_SIL: exec_sil(stat->args[0].ival); break;
        case S_IXA: exec_ixa(stat->args[0].ival); break;
        case S_STO: exec_sto(stat->args[0].ival, stat->args[1].ival); break;
        case S_IST: exec_ist(); break;
        case S_JMF: exec_jmf(stat->args[0].ival); break;
        case S_JMP: exec_jmp(stat->args[0].ival); break;
        case S_EQU: exec_equ(); break;
        case S_NEQ: exec_neq(); break;
        case S_CGT: exec_cgt(); break;
        case S_IGT: exec_igt(); break;
        case S_RGT: exec_rgt(); break;
        case S_SGT: exec_sgt(); break;
        case S_CGE: exec_cge(); break;
        case S_IGE: exec_ige(); break;
        case S_RGE: exec_rge(); break;
        case S_SGE: exec_sge(); break;
        case S_CLT: exec_clt(); break;
        case S_ILT: exec_ilt(); break;
        case S_RLT: exec_rlt(); break;
        case S_SLT: exec_slt(); break;
        case S_CLE: exec_cle(); break;
        case S_ILE: exec_ile(); break;
        case S_RLE: exec_rle(); break;
        case S_SLE: exec_sle(); break;
        case S_IN: exec_in(); break;
        case S_IPLUS: exec_iplus(); break;
        case S_RPLUS: exec_rplus(); break;
        case S_IMINUS: exec_iminus(); break;
        case S_RMINUS: exec_rminus(); break;
        case S_ITIMES: exec_itimes(); break;
        case S_RTIMES: exec_rtimes(); break;
        case S_IDIV: exec_idiv(); break;
        case S_RDIV: exec_rdiv(); break;
        case S_IUMI: exec_iumi(); break;
        case S_RUMI: exec_rumi(); break;
        case S_NEG: exec_neg(); break;
        case S_WR: exec_wr(stat->args[0].sval); break;
        case S_FWR: exec_fwr(stat->args[0].sval); break;
        case S_RD: exec_rd(stat->args[0].sval); break;
        case S_FRD: exec_frd(stat->args[0].sval); break;
        case S_TOINT: exec_toint(); break;
        case S_TOREAL: exec_toreal(); break;
        case S_READ: exec_read(stat->args[0].ival, stat->args[1].ival, stat->args[2].sval); break;
        case S_FREAD: exec_fread(stat->args[0].ival, stat->args[1].ival, stat->args[2].sval); break;
        case S_WRITE: exec_write(stat->args[0].sval); break;
        case S_FWRITE: exec_fwrite(stat->args[0].sval); break;
        case S_FUNC: exec_func(stat->args[0].ival); break;
        case S_RETURN: exec_return(); break;
        default: machine_error("Unknown operator"); break; }
}

void exec_toint(){
    int n = (float)pop_int();
    push_real(n);
}

void exec_toreal(){
    int n = (int)pop_real();
    push_int(n);
}

void exec_pop(){
    
}

void exec_ist(){
    
}

void exec_equ(){
    void * n, * m;
    if (top_ostack()->mode == EMB){
        n = &(top_ostack()->inst);
        m = &(under_top_ostack()->inst);
    }
    else{
        n = (top_ostack()->inst).sval;
        m = (under_top_ostack()->inst).sval;
    }
    int res = memcmp(n, m, top_ostack()->size);
    pop_ostack();
    pop_ostack();
    push_bool(res);
}

void exec_neq(){
    void * n, * m;
    if (top_ostack()->mode == EMB){
        n = &(top_ostack()->inst);
        m = &(under_top_ostack()->inst);
    }
    else{
        n = (top_ostack()->inst).sval;
        m = (under_top_ostack()->inst).sval;
    }
    int res = !memcmp(n, m, top_ostack()->size);
    pop_ostack();
    pop_ostack();
    push_bool(res);
}

void exec_cgt(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m>n);
}

void exec_igt(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m>n);
}

void exec_rgt(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m>n);
}

void exec_sgt(){
    char * n, * m;
    n = pop_char_p();
    m = pop_char_p();
    push_bool(strcmp(m, n) > 0);
}

void exec_cge(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m>=n);
}

void exec_ige(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m>=n);
}

void exec_rge(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m>=n);
}

void exec_sge(){
    char * n, * m;
    n = pop_char_p();
    m = pop_char_p();
    push_bool(strcmp(m, n) >= 0);
}

void exec_clt(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m<n);
}

void exec_ilt(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m<n);
}

void exec_rlt(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m<n);
    
}

void exec_slt(){
    char * n, * m;
    n = pop_char_p();
    m = pop_char_p();
    push_bool(strcmp(m, n) < 0);
}

void exec_cle(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m<=n);
}

void exec_ile(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m<=n);
}

void exec_rle(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m<=n);
}

void exec_sle(){
    char * n, * m;
    n = pop_char_p();
    m = pop_char_p();
    push_bool(strcmp(m, n) <= 0);
}

void exec_in(){
    
}

void exec_iplus(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m+n);
}

void exec_rplus(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m+n);
}

void exec_iminus(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m-n);
}

void exec_rminus(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m-n);
}

void exec_itimes(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m*n);
}

void exec_rtimes(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m*n);
}

void exec_idiv(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m/n);
}

void exec_rdiv(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m/n);
}

void exec_iumi(){
    int i = pop_int();
    push_int(-i);
}

void exec_rumi(){
    float r = pop_real();
    push_real(-r);
}

void exec_neg(){
    int n = pop_bool();
    push_bool(!n);
}

void exec_return(){
    pc = top_astack()->raddr;
}

void exec_read(int arg1, int arg2, char * arg3){
    
}

void exec_fread(int arg1, int arg2, char * arg3){
    
}

void exec_push(int arg1, int arg2, int raddr){
    
}

void exec_sto(int env_offset, int oid){
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_store = (a_declaration->objects) + oid;
    memcpy(&(o_to_store->inst), &(top_ostack()->inst), o_to_store->size);
    pop_ostack();
}

void exec_lda(int arg1, int arg2){
    
}

void exec_cat(int arg1, int arg2){
    
}

void exec_lod(int env_offset, int oid){
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_lod = (a_declaration->objects) + oid;
    push_ostack();
    memcpy(top_ostack(), o_to_lod, sizeof(Odescr));
}

void exec_write(char* arg1){
    
}

void exec_fwrite(char* arg1){
    
}

void exec_rd(char* arg1){
    
}

void exec_frd(char* arg1){
    
}

void exec_wr(char* arg1){
    
}

void exec_fwr(char* arg1){
    
}

void exec_lds(char* arg1){
    
}

void exec_ldr(float arg1){
    push_real(arg1);
}

void exec_ldc(char arg1){
    push_char(arg1);
}

void exec_func(int arg1){
    
}

void exec_goto(int arg1){
    pc = arg1;
}

void exec_jmf(int offset){
    if(!pop_bool())
        pc += offset-1;
}

void exec_jmp(int offset){
    pc += offset-1;
}

void exec_ixa(int arg1){
    
}

void exec_eil(int arg1){
    
}

void exec_sil(int arg1){
    
}

void exec_fda(int arg1){
    
}

void exec_ldi(int arg1){
    push_int(arg1);
}

void exec_new(int arg1){
    Odescr *po;
    po = push_ostack();
    po->mode = EMB;
    po->size = arg1;
}

void exec_news(int arg1){
    
}

