#ifndef S_MACHINE
#define S_MACHINE

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    Operator op;
    Lexval args[MAXARGS];
} Scode;
Scode *prog;

typedef struct adescr {
    int numobj;
    Odescr *objects;
    int raddr;
    struct adescr *alink;
} Adescr;
Adescr **astack;

typedef enum {
    EMB,
    STA
} Mode;

typedef struct {
    Mode mode;
    int size;
    Lexval inst;
} Odescr;
Odescr ** ostack;

char *istack;
extern Scode *prog;
extern int pc;
Adescr **astack;
Odescr **ostack;
char *istack;
int asize, osize, isize;
int ap, op, ip;
long size_allocated = 0, size_deallocated = 0;

void start_machine();
void end_machine();
void *newmem(int size);
void freemem(char *p, int size);

Adescr *push_astack();
void pop_astack();

Scode *prog;
int pc;
void exec(Scode *stat);

void exec_goto(int addr);
void exec_jmp(int offset);
void exec_jmf(int offset);
void exec_return();
void exec_iplus();
void exec_igt();
void exec_new(int size);

#endif