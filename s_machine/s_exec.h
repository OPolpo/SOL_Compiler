#ifndef S_EXEC
#define S_EXEC

#include <stdio.h>
#include <stdlib.h>
#include "s_machine.h"

void exec(Scode *stat);

void exec_goto(int addr);
void exec_jmp(int offset);
void exec_jmf(int offset);
void exec_return();
void exec_iplus();
void exec_igt();
void exec_new(int size);

void exec_news(stat->args[0].ival);
void exec_ldc(stat->args[0].ival);
void exec_ldi(stat->args[0].ival);
void exec_lds(stat->args[0].sval);
void exec_ldr(stat->args[0].rval);
void exce_lod(stat->args[0].ival);
void exce_cat(stat->args[0].ival);
void exce_lda(stat->args[0].ival);
void exce_fda(stat->args[0].ival);
void exce_eil(stat->args[0].ival);
void exce_sil(stat->args[0].ival);
void exce_ixa(stat->args[0].ival);
void exce_sto(stat->args[0].ival);
void exce_ist(stat->args[0].ival);
void exce_equ(stat->args[0].ival);
void exce_neq(stat->args[0].ival);
void exce_cgt(stat->args[0].ival);
void exce_rgt(stat->args[0].ival);
void exce_sgt(stat->args[0].ival);
void exce_cge(stat->args[0].ival);
void exce_ige(stat->args[0].ival);
void exce_rge(stat->args[0].ival);
void exce_sge(stat->args[0].ival);
void exce_clt(stat->args[0].ival);
void exce_ilt(stat->args[0].ival);
void exce_rlt(stat->args[0].ival);
void exce_slt(stat->args[0].ival);
void exce_cle(stat->args[0].ival);
void exce_ile(stat->args[0].ival);
void exce_rle(stat->args[0].ival);
void exce_sle(stat->args[0].ival);
void exce_in(stat->args[0].ival);
void exce_rplus(stat->args[0].ival);
void exce_iminus(stat->args[0].ival);
void exce_rminus(stat->args[0].ival);
void exce_itimes(stat->args[0].ival);
void exce_rtimes(stat->args[0].ival);
void exce_idiv(stat->args[0].ival);
void exce_rdiv(stat->args[0].ival);
void exce_iumi(stat->args[0].ival);
void exce_rumi(stat->args[0].ival);
void exce_neg(stat->args[0].ival);
void exce_wr(stat->args[0].ival);
void exce_fwr(stat->args[0].ival);
void exce_push(stat->args[0].ival);
void exce_pop(stat->args[0].ival);
void exce_Rd(stat->args[0].ival);
void exce_frd(stat->args[0].ival);
void exce_toin(stat->args[0].ival);
void exce_toreal(stat->args[0].ival);
void exce_read(stat->args[0].ival);
void exce_fread(stat->args[0].ival);
void exce_write(stat->args[0].ival);
void exce_fwrite(stat->args[0].ival);
void exce_func(stat->args[0].ival);
void exce_halt(stat->args[0].ival);




#endif