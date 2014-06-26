#ifndef S_EXEC
#define S_EXEC

#include <stdio.h>
#include <stdlib.h>
#include "s_machine.h"

void exec(Scode *stat);

void exec_push(int arg1, int arg2, int pc);

void exec_toint();
void exec_toreal();
void exec_pop();
void exec_ist();
void exec_equ();
void exec_neq();
void exec_igt();
void exec_cgt();
void exec_rgt();
void exec_sgt();
void exec_cge();
void exec_ige();
void exec_rge();
void exec_sge();
void exec_clt();
void exec_ilt();
void exec_rlt();
void exec_slt();
void exec_cle();
void exec_ile();
void exec_rle();
void exec_sle();
void exec_in();
void exec_iplus();
void exec_rplus();
void exec_iminus();
void exec_rminus();
void exec_itimes();
void exec_rtimes();
void exec_idiv();
void exec_rdiv();
void exec_iumi();
void exec_rumi();
void exec_neg();
void exec_return();

void exec_read(int arg1, int arg2, char * arg3);
void exec_fread(int arg1, int arg2, char * arg3);

void exec_sto(int arg1, int arg2);
void exec_lda(int arg1, int arg2);
void exec_cat(int arg1, int arg2);
void exec_lod(int arg1, int arg2);

void exec_write(char* arg1);
void exec_fwrite(char* arg1);
void exec_rd(char* arg1);
void exec_frd(char* arg1);
void exec_wr(char* arg1);
void exec_fwr(char* arg1);
void exec_lds(char* arg1);

void exec_ldr(float arg1);

void exec_ldc(char arg1);

void exec_func(int arg1);
void exec_goto(int arg1);
void exec_jmf(int arg1);
void exec_jmp(int arg1);
void exec_ixa(int arg1);
void exec_eil(int arg1);
void exec_sil(int arg1);
void exec_fda(int arg1);
void exec_ldi(int arg1);
void exec_new(int arg1);
void exec_news(int arg1);

#endif