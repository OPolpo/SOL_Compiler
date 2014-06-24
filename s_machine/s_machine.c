#include "s_machine.h"

extern Scode *prog;
extern int pc;
Adescr **astack;
Odescr **ostack;
char *istack;
int asize, osize, isize, code_size;
int ap, op, ip; // point to he first free elements of their stack
long size_allocated = 0, size_deallocated = 0;

int main(int argc, char* argv[]){
	Scode * stat;
	start_machine();
	while((stat = &prog[pc++])->op != S_HALT)
        exec(stat);
    end_machine();
    return 1;
}

void start_machine() {
    load_scode();
    pc = ap = op = ip = 0;
    astack = (Adescr**)newmem(sizeof(Adescr*)*ASTACK_UNIT);
    asize = ASTACK_UNIT;
    ostack = (Odescr**)newmem(sizeof(Odescr*)*OSTACK_UNIT);
    osize = OSTACK_UNIT;
    istack = (char*)newmem(ISTACK_UNIT);
    isize = ISTACK_UNIT;
}

void end_machine() {
    freemem((char*)prog, sizeof(Scode)*code_size);
    freemem((char*)astack, sizeof(Adescr*)*asize);
    freemem((char*)ostack, sizeof(Odescr*)*osize);
    freemem(istack, isize);
    printf("Program executed without errors\n");
    printf("Allocation: %ld bytes\n", size_allocated);
    printf("Deallocation: %ld bytes\n", size_deallocated);
    printf("Residue: %ld bytes\n", size_allocated - size_deallocated);
}

void * newmem(int size) {
    void *p;
    if((p = malloc(size)) == NULL) machine_error("Failure in memory allocation");
    size_allocated += size;
    return p; }

void freemem(char *p, int size) {
    free(p);
    size_deallocated += size;
}

Adescr * top_astack(){
    if (ap==0) {
        machine_error("top_astack");
    }
    return astack[ap-1];
}

Adescr * push_astack(){
    Adescr **old_astack; int i;
    if(ap == asize) {
        old_astack = astack;
        astack = (Adescr**) newmem(sizeof(Adescr*)*(asize + ASTACK_UNIT));
        for(i = 0; i < asize; i++)
            astack[i] = old_astack[i];
        freemem((char*)old_astack, sizeof(Adescr*)*asize);
        asize += ASTACK_UNIT;
    }
    return (astack[ap++] = (Adescr*)newmem(sizeof(Adescr)));
}

void pop_astack() {
    if(ap == 0) machine_error("pop_adescr()");
    freemem((char*)astack[--ap], sizeof(Adescr));
}

Odescr * top_ostack(){
    if (op==0) {
        machine_error("top_ostack");
    }
    return ostack[op-1];
}

Odescr * push_ostack(){
    Odescr **old_ostack; int i;
    if(op == osize) {
        old_ostack = ostack;
        ostack = (Odescr**) newmem(sizeof(Odescr*)*(osize + OSTACK_UNIT));
        for(i = 0; i < osize; i++)
            ostack[i] = old_ostack[i];
        freemem((char*)old_ostack, sizeof(Odescr*)*osize);
        osize += OSTACK_UNIT;
    }
    return (ostack[op++] = (Odescr*)newmem(sizeof(Odescr)));
}

void pop_ostack() {
    if(op == 0) machine_error("pop_odescr()");
    freemem((char*)ostack[--op], sizeof(Odescr));
}

int pop_int(){
    int i = top_ostack()->inst.ival;
    pop_ostack();
    return i;
}

void push_int(int i){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(int);
    new_o->inst.ival = i;
}

float pop_real(){
    float r = top_ostack()->inst.rval;
    pop_ostack();
    return r;
}

void push_real(float r){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(float);
    new_o->inst.rval = r;
}

char pop_char(){
    char c = top_ostack()->inst.cval;
    pop_ostack();
    return c;
}

void push_char(char c){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char);
    new_o->inst.cval = c;
}

int pop_bool(){
    int i = top_ostack()->inst.cval;
    pop_ostack();
    return i;
}

void push_bool(int b){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char);
    new_o->inst.cval = b;
}