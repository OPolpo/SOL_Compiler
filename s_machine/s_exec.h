#ifndef S_EXEC
#define S_EXEC

#include <stdio.h>
#include <stdlib.h>
#include "s_machine.h"

void exec(Scode *stat);

void exce_toint();
void exce_toreal();
void exec_pop();
void exce_ist();
void exce_equ();
void exce_neq();
void exec_igt();
void exce_cgt();
void exce_rgt();
void exce_sgt();
void exce_cge();
void exce_ige();
void exce_rge();
void exce_sge();
void exce_clt();
void exce_ilt();
void exce_rlt();
void exce_slt();
void exce_cle();
void exce_ile();
void exce_rle();
void exce_sle();
void exce_in();
void exec_iplus();
void exce_rplus();
void exce_iminus();
void exce_rminus();
void exce_itimes();
void exce_rtimes();
void exce_idiv();
void exce_rdiv();
void exce_iumi();
void exce_rumi();
void exce_neg();
void exec_return();

void exce_read(int arg1, int arg2, char * arg3);
void exce_fread(int arg1, int arg2, char * arg3);

void exce_push(int arg1, int arg2);
void exce_sto(int arg1, int arg2);
void exce_lda(int arg1, int arg2);
void exce_cat(int arg1, int arg2);
void exce_lod(int arg1, int arg2);

void exce_write(char* arg1);
void exce_fwrite(char* arg1);
void exce_Rd(char* arg1);
void exce_frd(char* arg1);
void exce_wr(char* arg1);
void exce_fwr(char* arg1);
void exec_lds(char* arg1);

void exec_ldr(float arg1);

void exec_ldc(char arg1);


void exce_func(int arg1);
void exec_goto(int arg1);
void exec_jmf(int arg1);
void exec_jmp(int arg1);
void exce_ixa(int arg1);
void exce_eil(int arg1);
void exce_sil(int arg1);
void exce_fda(int arg1);
void exec_ldi(int arg1);
void exec_new(int arg1);
void exec_news(int arg1);





#endif