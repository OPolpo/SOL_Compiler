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

#endif