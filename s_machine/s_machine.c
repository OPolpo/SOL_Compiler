#include "s_machine.h"
#include "s_exec.h"

extern Scode *prog;
extern int pc;

Adescr **astack;
Odescr **ostack;
char *istack;
int asize, osize, isize, code_size;
int ap, op, ip; // point to he first free elements of their stack
long size_allocated = 0, size_deallocated = 0;

Str_c_node ** str_const_table;

int main(int argc, char* argv[]){
	Scode * stat;
	start_machine(argv[1]);
    
	while((stat = &prog[pc++])->op != S_HALT)
        exec(stat);
    end_machine();
    return 0;
}

void start_machine(char * input) {
    str_const_table = (Str_c_node **)newmem(sizeof(Str_c_node*)*STR_CONST_DIM);
    FILE *input_file = fopen(input, "r");
    if (!input_file){
        machine_error("ERRORE nel caricamento del file");
    }
    load_scode(input_file);
    fclose (input_file);
    
    pc = ap = op = ip = 0;
    astack = (Adescr**)newmem(sizeof(Adescr*)*ASTACK_UNIT);
    asize = ASTACK_UNIT;
    ostack = (Odescr**)newmem(sizeof(Odescr*)*OSTACK_UNIT);
    osize = OSTACK_UNIT;
    istack = (char*)newmem(ISTACK_UNIT);
    isize = ISTACK_UNIT;
}

void end_machine() {
    //printf("%d: %d %f %c %s\n",top_ostack()->size,top_ostack()->inst.ival,top_ostack()->inst.rval,top_ostack()->inst.cval,top_ostack()->inst.sval);
    //printf("%d: %d %f %c %s\n",under_top_ostack()->size,under_top_ostack()->inst.ival,under_top_ostack()->inst.rval,under_top_ostack()->inst.cval,under_top_ostack()->inst.sval);
    //pop_ostack();
    //pop_ostack();
    freemem((char*)prog, sizeof(Scode)*code_size);
    freemem((char*)astack, sizeof(Adescr*)*asize);
    freemem((char*)ostack, sizeof(Odescr*)*osize);
    freemem(istack, isize);
    free_str_c_table();
    
    printf("Program executed without errors\n");
    printf("Allocation: %ld bytes\n", size_allocated);
    printf("Deallocation: %ld bytes\n", size_deallocated);
    printf("Residue: %ld bytes\n", size_allocated - size_deallocated);
}

void * newmem(int size) {
    void *p;
    if((p = calloc(1,size)) == NULL) machine_error("Failure in memory allocation");
    size_allocated += size;
    return p;
}

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

Odescr ** get_p2objects(int i){
    return &ostack[i];
}
int get_next_op(){
    return op;
}

Odescr * top_ostack(){
    if (op==0) {
        machine_error("top_ostack");
    }
    return ostack[op-1];
}

Odescr * over_top_ostack(){
    return top_ostack()+sizeof(Odescr *);
}

Odescr * under_top_ostack(){
    if (op<=1) {
        machine_error("under_top_ostack");
    }
    return ostack[op-2];
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

char * push_istack(int size){
    char *old_istack; int i;
    if(ip+size >= isize) {
        old_istack = istack;
        istack = newmem(isize + ISTACK_UNIT);
        for(i = 0; i < isize; i++)
            istack[i] = old_istack[i];
        freemem(old_istack, isize);
        isize += ISTACK_UNIT;
    }
    ip+=size;
    return &(istack[ip-size]);
}

void pop_istack(int size) {
    if(ip < size) machine_error("pop_istack()");
    ip = ip-size;
}

void move_down_istack(int to_move, int this_much){
    
    printf("hai chiamato move_down_istack\n");
    if(ip - to_move - this_much < 0) machine_error("move_down_istack()");
    if (to_move<0 || this_much<0) machine_error("move_down_istack() parameters");
    memmove(&istack[ip-to_move-this_much], &istack[ip-to_move], to_move);
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
    int i = (top_ostack()->inst.cval != '0');
    if (i && top_ostack()->inst.cval != '1') {
        machine_error("pop_bool()");
    }
    pop_ostack();
    return i;
}

void push_bool(int b){
    Odescr * new_o = push_ostack();
    new_o->mode = EMB;
    new_o->size = sizeof(char);
    new_o->inst.cval = b ? '1' :'0';
}

char * pop_string(){
    char * s = top_ostack()->inst.sval;
    pop_ostack();
    return s;
}

void push_string(char * s){ //assuming is "mallocated" and is in the hash table
    Odescr * new_o = push_ostack();
    new_o->mode = STA;
    new_o->size = (int)strlen(s)+1;
    new_o->inst.sval = insert_str_c(s);
}


//manage str_const_table
char * insert_str_c(char * s){
    char *without = s;
    printf("vorrei inserire %s, pulisco e ho: %s\n", s, without);
    
    
    char * p_on_table = get_str_c(without);
    if (!p_on_table){
        int pos = hash_str_c(without);
        Str_c_node * new_node = (Str_c_node *)newmem(sizeof(Str_c_node));
        new_node->string = (char*)newmem(((int)strlen(without)+1) * sizeof(char));
        
        //new_node->string = calloc(strlen(s)+1, sizeof(char));
        strcpy(new_node->string, without);
        p_on_table = new_node->string;
        new_node->next = str_const_table[pos];
        str_const_table[pos] = new_node;
    }
    return p_on_table;
}

char * insert_strWclean(char * s){
    char * without = s+1;
    without[strlen(without)-1]=0;
    
    return insert_str_c(without);
}

char * get_str_c(char * s){
    int pos = hash_str_c(s);
    Str_c_node * node = str_const_table[pos];
    while (node) {
        if (strcmp(node->string, s) == 0) {
            return node->string;
        }
        node = node->next;
    }
    return NULL;
}

int hash_str_c(char * s){
    int i, h = 0;
    for(i = 0; s[i] != '\0'; i++){
        h = ((h << STR_SHIFT) + s[i]) % STR_CONST_DIM;
    }
    return h;
}

void free_str_c_table(){
    int i=0;
    for (i=0; i<STR_CONST_DIM; i++) {
        if (str_const_table[i]!=NULL) {
            Str_c_node * newhead = str_const_table[i]->next;
            while (newhead) {
                freemem((char *)str_const_table[i], sizeof(Str_c_node));
                str_const_table[i] = newhead;
                newhead = newhead->next;
            }
        }
    }
    freemem((char*)str_const_table, sizeof(Str_c_node*)*STR_CONST_DIM);
}

void machine_error(char * msg){
    fprintf(stderr, "Machine error: %s\n", msg);
    exit(EXIT_FAILURE);
}


